use std::cell::UnsafeCell;
use std::hint::spin_loop;
use std::ops::{Deref, DerefMut};
use std::sync::Arc;
use std::sync::atomic::{AtomicBool, Ordering};
use std::thread;

pub struct SpinLock<T> {
    lock: AtomicBool,
    data: UnsafeCell<T>,
}

pub struct LockGuard<'a, T> {
    lock: &'a SpinLock<T>,
}

impl<T> SpinLock<T> {
    pub fn new(data: T) -> Self {
        SpinLock {
            lock: AtomicBool::new(false),
            data: UnsafeCell::new(data),
        }
    }

    pub fn lock(&self) -> LockGuard<'_, T> {
        while self.lock.swap(true, Ordering::Acquire) {
            spin_loop();
        }

        LockGuard { lock: self }
    }
}

unsafe impl<T: Send> Sync for SpinLock<T> {}

impl<'a, T> Deref for LockGuard<'a, T> {
    type Target = T;

    fn deref(&self) -> &Self::Target {
        unsafe { &*self.lock.data.get() }
    }
}

impl<'a, T> DerefMut for LockGuard<'a, T> {
    fn deref_mut(&mut self) -> &mut Self::Target {
        unsafe { &mut *self.lock.data.get() }
    }
}

impl<'a, T> Drop for LockGuard<'a, T> {
    fn drop(&mut self) {
        self.lock.lock.store(false, Ordering::Release);
    }
}

fn main() {
    let counter = Arc::new(SpinLock::new(0));
    thread::scope(|s| {
        s.spawn(|| {
            for _ in 0..5 {
                let mut guard = counter.lock();
                *guard += 1;
            }
        });

        s.spawn(|| {
            for _ in 0..5 {
                let mut guard = counter.lock();
                *guard += 1;
            }
        });
    });

    println!("{}", *counter.lock());
}
