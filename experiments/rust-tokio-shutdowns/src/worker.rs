use std::sync::{
    Arc,
    atomic::{AtomicBool, Ordering},
};

use tokio::time::{Duration, sleep};
use tokio_util::sync::CancellationToken;

pub struct AtomicWorker {
    is_doing_work: Arc<AtomicBool>,
    work_is_done: Arc<AtomicBool>,
    stop_signal: Arc<AtomicBool>,
}

impl AtomicWorker {
    pub fn new(work_is_done: Arc<AtomicBool>, stop_signal: Arc<AtomicBool>) -> Self {
        Self {
            is_doing_work: Arc::new(AtomicBool::new(false)),
            work_is_done,
            stop_signal,
        }
    }

    pub async fn do_work(&self) {
        while !self.stop_signal.load(Ordering::Acquire) {
            self.is_doing_work.store(true, Ordering::Release);
            self.work_is_done.store(false, Ordering::Release);
            sleep(Duration::from_millis(0)).await;
            self.work_is_done.store(true, Ordering::Release);
            self.is_doing_work.store(false, Ordering::Release);
        }
    }
}

#[derive(Clone)]
pub struct TokenWorker {
    is_doing_work: Arc<AtomicBool>,
    work_is_done: Arc<AtomicBool>,
    cancellation_token: CancellationToken,
}

impl TokenWorker {
    pub fn new(work_is_done: Arc<AtomicBool>) -> Self {
        Self {
            is_doing_work: Arc::new(AtomicBool::new(false)),
            work_is_done,
            cancellation_token: CancellationToken::new(),
        }
    }

    pub async fn do_work(&self) {
        loop {
            self.is_doing_work.store(true, Ordering::Release);
            self.work_is_done.store(false, Ordering::Release);

            tokio::select! {
                _ = sleep(Duration::from_millis(0)) => {
                    self.work_is_done.store(true, Ordering::Release);
                    self.is_doing_work.store(false, Ordering::Release);
                }
                _ = self.cancellation_token.cancelled() => {
                    self.work_is_done.store(true, Ordering::Release);
                    self.is_doing_work.store(false, Ordering::Release);
                    break;
                }
            }
        }
    }

    pub fn cancel(&mut self) {
        self.cancellation_token.cancel();
    }
}
