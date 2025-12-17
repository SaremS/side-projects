use std::cell::UnsafeCell;
use std::ops::{Deref, DerefMut};
use std::sync::Arc;
use std::sync::atomic::{AtomicUsize, Ordering};
use std::thread;

struct SPSCQueue<T: Clone> {
    data: UnsafeCell<Vec<Option<T>>>,

    capacity: usize,

    write_idx: AtomicUsize,
    read_idx: AtomicUsize,
}

unsafe impl<T: Clone> Sync for SPSCQueue<T> {}

impl<T: Clone> SPSCQueue<T> {
    pub fn new(capacity: usize) -> Self {
        SPSCQueue {
            data: UnsafeCell::new(vec![None; capacity]),
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
                let data_ref = &mut *self.data.get();
                data_ref[write_idx] = Some(item);
            }
            self.write_idx.store(next_write_idx, Ordering::Release);
            return true;
        }

        return false;
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

        let mut result = None;
        unsafe {
            let data_ref = &*self.data.get();
            result = data_ref[read_idx].clone();
        }

        self.read_idx.store(next_read_idx, Ordering::Release);
        return result;
    }
}

fn main() {
    let queue = Arc::new(SPSCQueue::<i32>::new(100));
    thread::scope(|s| {
        s.spawn(|| {
            for i in 0..200 {
                while (!queue.push(i)) {}
            }
        });

        s.spawn(|| {
            for i in 0..200 {
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
