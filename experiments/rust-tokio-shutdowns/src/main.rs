pub mod worker;
pub use worker::*;

use std::sync::Arc;

use tokio::time::{Duration, sleep};

#[tokio::main]
async fn main() {
    //atomic stop signal
    let work_is_done = Arc::new(std::sync::atomic::AtomicBool::new(false));
    let stop_signal = Arc::new(std::sync::atomic::AtomicBool::new(false));
    let worker = Arc::new(worker::AtomicWorker::new(
        work_is_done.clone(),
        stop_signal.clone(),
    ));

    let worker_clone = worker.clone();
    let handle = tokio::spawn(async move {
        worker_clone.do_work().await;
    });

    tokio::time::sleep(Duration::from_millis(50)).await;
    stop_signal.store(true, std::sync::atomic::Ordering::Release);

    let _ = handle.await;

    drop(worker);
    assert!(work_is_done.load(std::sync::atomic::Ordering::Acquire));

    //token stop signal
    let work_is_done = Arc::new(std::sync::atomic::AtomicBool::new(false));
    let worker = Arc::new(worker::TokenWorker::new(work_is_done.clone()));

    let worker_clone = worker.clone();
    let handle = tokio::spawn(async move {
        worker_clone.do_work().await;
        worker_clone
    });

    tokio::time::sleep(Duration::from_millis(50)).await;

    worker.as_ref().clone().cancel();
    let _ = handle.await;

    assert!(work_is_done.load(std::sync::atomic::Ordering::Acquire));
}
