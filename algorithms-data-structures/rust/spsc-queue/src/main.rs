use std::cell::UnsafeCell;
use std::mem::MaybeUninit;
use std::sync::Arc;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::thread;

struct SPSCQueue<T> {
    buffer: UnsafeCell<Vec<MaybeUninit<T>>>,

    capacity: usize,

    write_idx: AtomicUsize,
    read_idx: AtomicUsize,
}

unsafe impl<T> Sync for SPSCQueue<T> {}

impl<T> SPSCQueue<T> {
    pub fn new(capacity: usize) -> Self {
        let buffer: Vec<MaybeUninit<T>> = (0..capacity).map(|_| MaybeUninit::uninit()).collect();
        SPSCQueue {
            buffer: UnsafeCell::new(buffer),
            capacity,
            write_idx: AtomicUsize::new(0),
            read_idx: AtomicUsize::new(0),
        }
    }

    pub fn push(&self, item: T) -> bool {
        let write_idx = self.write_idx.load(Ordering::Relaxed);
        let mut next_write_idx = write_idx + 1;

        if next_write_idx == self.capacity {
            next_write_idx = 0;
        }

        if next_write_idx != self.read_idx.load(Ordering::Acquire) {
            unsafe {
                let data_ptr = (*self.buffer.get()).as_mut_ptr();
                let mut inner = MaybeUninit::<T>::uninit();
                inner.write(item);
                data_ptr.add(write_idx).write(inner);
            }
            self.write_idx.store(next_write_idx, Ordering::Release);
            return true;
        }

        false
    }

    pub fn pop(&self) -> Option<T> {
        let read_idx = self.read_idx.load(Ordering::Relaxed);
        if read_idx == self.write_idx.load(Ordering::Acquire) {
            return None;
        }
        let mut next_read_idx = read_idx + 1;

        if next_read_idx == self.capacity {
            next_read_idx = 0;
        }

        let result;

        unsafe {
            let data_ptr = (*self.buffer.get()).as_ptr();
            result = Some(data_ptr.add(read_idx).read().assume_init());
        }

        self.read_idx.store(next_read_idx, Ordering::Release);
        result
    }
}

fn main() {
    let queue = Arc::new(SPSCQueue::<i32>::new(100));
    thread::scope(|s| {
        s.spawn(|| {
            for i in 0..200 {
                while !queue.push(i) {}
            }
        });

        s.spawn(|| {
            for _ in 0..200 {
                loop {
                    if let Some(data) = queue.pop() {
                        println!("{}", data);
                        break;
                    }
                }
            }
        });
    });
}
