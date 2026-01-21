use std::fs;
use std::time::Instant;

fn main() {
    let test_path: &str = "src/test.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test ");
    part_one(test_path);
    print!("Input ");
    part_one(input_path);

    println!("========= PART TWO ==========");
    print!("Test ");
    part_two(test_path);
    print!("Input ");
    part_two(input_path);
}

// Used to read text file line by line

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem
    let lines: Vec<String> = fs::read_to_string(file_path)
        .unwrap()
        .lines()
        .map(|l| l.parse().unwrap())
        .collect();
    let goal_timestamp: u64 = lines[0].parse().unwrap();
    let bus_ids: Vec<(u64, u64)> = lines[1]
        .split(',')
        .enumerate()
        .filter(|(_, s)| *s != "x")
        .map(|(i, s)| (i as u64, s.parse().unwrap()))
        .collect();

    // old solution
    // let (best_id, best_wait): (u64, u64) = bus_ids.iter().fold(
    //     (0, std::u64::MAX),
    //     |(mut id, mut time_to_wait), (_, bus_id)| {
    //         let next_time_to_wait: u64 = (bus_id - goal_timestamp % bus_id) % bus_id;
    //         if next_time_to_wait < time_to_wait {
    //             time_to_wait = next_time_to_wait;
    //             id = *bus_id;
    //         }
    //         (id, time_to_wait as u64)
    //     },
    // );

    let (best_id, best_wait) = bus_ids
        .iter()
        .map(|(_, bus_id)| {
            let wait_time = (bus_id - goal_timestamp % bus_id) % bus_id;
            (bus_id, wait_time)
        })
        .min_by_key(|&(_id, wait)| wait)
        .unwrap();

    let answer = best_id * best_wait;
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();

    // Parse the problem
    let lines: Vec<String> = fs::read_to_string(file_path)
        .unwrap()
        .lines()
        .map(|l| l.parse().unwrap())
        .collect();
    let bus_ids: Vec<(u64, u64)> = lines[1]
        .split(',')
        .enumerate()
        .filter(|(_, s)| *s != "x")
        .map(|(i, s)| (i as u64, s.parse().unwrap()))
        .collect();

    // solve
    let (time, _step): (u64, u64) =
        bus_ids
            .iter()
            .skip(1)
            .fold((0u64, bus_ids[0].1), |(time, step), &(offset, bus_id)| {
                let mut new_time = time;
                while ((new_time + offset) % bus_id) != 0 {
                    new_time += step;
                }
                (new_time, step * bus_id)
            });

    let answer = time;
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
