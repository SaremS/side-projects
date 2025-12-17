use std::cell::UnsafeCell;
use std::hint::spin_loop;
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

pub struct SpinLock {
    lock: AtomicBool,
}

impl Default for SpinLock {
    fn default() -> Self {
        Self::new()
    }
}

impl SpinLock {
    pub fn new() -> Self {
        SpinLock {
            lock: AtomicBool::new(false),
        }
    }

    pub fn lock(&self) {
        while self.lock.swap(true, Ordering::Acquire) {
            spin_loop();
        }
    }

    pub fn unlock(&self) {
        self.lock.store(false, Ordering::Release);
    }
}

struct UnsafeShared<T> {
    inner: UnsafeCell<T>,
}

unsafe impl<T> Sync for UnsafeShared<T> {}

impl<T> UnsafeShared<T> {
    fn new(val: T) -> Self {
        Self {
            inner: UnsafeCell::new(val),
        }
    }
}

fn main() {
    let counter = Arc::new(UnsafeShared::new(0));
    let spin_lock = SpinLock::new();
    thread::scope(|s| {
        s.spawn(|| {
            for _ in 0..5 {
                spin_lock.lock();
                unsafe {
                    let ptr = counter.inner.get();
                    *ptr += 1;
                }
                spin_lock.unlock();
            }
        });

        s.spawn(|| {
            for _ in 0..5 {
                spin_lock.lock();
                unsafe {
                    let ptr = counter.inner.get();
                    *ptr += 1;
                }
                spin_lock.unlock();
            }
        });
    });

    unsafe { println!("{}", *counter.inner.get()) };
}
