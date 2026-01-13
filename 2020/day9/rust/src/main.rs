use std::fs;
use std::io::{self, BufRead};
use std::ops::Add;
use std::path::Path;
use std::time::Instant;

fn main() {
    let test_path: &str = "src/test.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test ");
    part_one(test_path, 5);
    print!("Input ");
    part_one(input_path, 25);

    println!("========= PART TWO ==========");
    print!("Test ");
    part_two(test_path, 5);
    print!("Input ");
    part_two(input_path, 25);
}

// Used to read text file line by line
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<fs::File>>>
where
    P: AsRef<Path>,
{
    let file = fs::File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

#[allow(dead_code)]
fn part_one(file_path: &str, premable_size: usize) {
    let now = Instant::now();

    let mut answer: u64 = 0;
    // Parse the problem
    let data: Vec<u64> = fs::read_to_string(file_path)
        .expect("File path wrong")
        .lines()
        .map(|l| l.parse().unwrap())
        .collect();

    // solve
    // NOTE: This could have been solved with a .find_map()
    for window in data.windows(premable_size + 1) {
        let (premable, target) = (&window[..premable_size], window[premable_size]);
        let mut visited: Vec<u64> = Vec::new();
        let mut is_protected = false;
        for &val in premable {
            if let Some(complement) = target.checked_sub(val) {
                if visited.contains(&complement) {
                    is_protected = true;
                    break;
                }
            }
            visited.push(val.clone());
        }

        if !is_protected {
            answer = target.clone();
            break;
        }
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

#[allow(dead_code)]
fn part_two(file_path: &str, premable_size: usize) {
    let now = Instant::now();

    let mut answer: u64 = 0;
    // Parse the problem
    let data: Vec<u64> = fs::read_to_string(file_path)
        .expect("File path wrong")
        .lines()
        .map(|l| l.parse().unwrap())
        .collect();

    // solve
    let mut result: Option<(usize, u64)> = None;
    for (idx, window) in data.windows(premable_size + 1).enumerate() {
        let (premable, target) = (&window[..premable_size], window[premable_size]);
        let mut visited: Vec<u64> = Vec::new();
        let mut is_protected = false;
        for &val in premable {
            if let Some(complement) = target.checked_sub(val) {
                if visited.contains(&complement) {
                    is_protected = true;
                    break;
                }
            }
            visited.push(val);
        }

        if !is_protected {
            result = Some((idx, target));
            break;
        }
    }

    // NOTE: To myself - I should avoid setting initial states in case the input
    // is unexpectadely small. In this case, it could be avoided by iterating forwards instead of
    // backwards.
    let (invalid_idx, target_number) = result.expect("No value found");
    let mut right_idx = &invalid_idx - 1;
    let mut left_idx = &invalid_idx - 2;
    let mut current_sum: u64 = data[right_idx] + data[left_idx];
    while current_sum != target_number {
        if right_idx == left_idx {
            left_idx -= 1;
            current_sum += data[left_idx];
        }

        if current_sum < target_number {
            left_idx -= 1;
            current_sum += data[left_idx];
        } else if current_sum > target_number {
            current_sum -= data[right_idx];
            right_idx -= 1;
        }
    }

    let (min, max) = &data[left_idx..=right_idx]
        .iter()
        .fold((u64::MAX, u64::MIN), |(min, max), &val| {
            (min.min(val), max.max(val))
        });

    answer = min + max;
    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
