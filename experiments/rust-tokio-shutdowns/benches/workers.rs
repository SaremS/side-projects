use criterion::{Criterion, criterion_group, criterion_main};
use std::hint::black_box;
use std::sync::{
    Arc,
    atomic::{AtomicBool, Ordering},
};
use tokio::runtime::Runtime;

use rust_tokio_shutdowns::worker::{AtomicWorker, TokenWorker};

fn bench_worker_lifecycle(c: &mut Criterion) {
    let rt = Runtime::new().unwrap();

    let work_is_done = Arc::new(AtomicBool::new(false));
    let stop_signal = Arc::new(AtomicBool::new(false));
    let worker = Arc::new(AtomicWorker::new(work_is_done.clone(), stop_signal.clone()));

    c.bench_function("atomic_worker_spawn_and_stop", |b| {
        b.to_async(&rt).iter(|| async {
            let worker_clone = worker.clone();

            let handle = tokio::spawn(async move {
                worker_clone.do_work().await;
            });

            stop_signal.store(true, Ordering::Release);

            let _ = black_box(handle.await);
        });
    });

    let work_is_done = Arc::new(AtomicBool::new(false));
    let worker = Arc::new(TokenWorker::new(work_is_done.clone()));

    c.bench_function("token_worker_spawn_and_stop", |b| {
        b.to_async(&rt).iter(|| async {
            let worker_clone = worker.clone();

            let handle = tokio::spawn(async move {
                worker_clone.do_work().await;
            });

            worker.as_ref().clone().cancel();
            let _ = black_box(handle.await);
        });
    });
}

criterion_group!(benches, bench_worker_lifecycle);
criterion_main!(benches);
