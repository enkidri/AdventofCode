use std::fs;
use std::time::Instant;

fn main() {
    let test_path: &str = "src/test.txt";
    let test_path1: &str = "src/test1.txt";
    let test_path2: &str = "src/test2.txt";
    let test_path3: &str = "src/test3.txt";
    let test_path4: &str = "src/test4.txt";
    let test_path5: &str = "src/test5.txt";
    let test_path6: &str = "src/test6.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test ");
    part_one(test_path);
    part_one(test_path1);
    part_one(test_path2);
    part_one(test_path3);
    part_one(test_path4);
    part_one(test_path5);
    part_one(test_path6);
    print!("Input ");
    part_one(input_path);

    println!("========= PART TWO ==========");
    print!("Test ");
    part_two(test_path);
    part_two(test_path1);
    part_two(test_path2);
    part_two(test_path3);
    part_two(test_path4);
    part_two(test_path5);
    part_two(test_path6);
    print!("Input ");
    part_two(input_path);
}

// Used to read text file line by line

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem
    let nums: Vec<i32> = fs::read_to_string(file_path)
        .unwrap()
        .trim()
        .split(',')
        .map(|d| d.parse().unwrap())
        .collect();

    let num_iter = 2020;
    let mut memo = vec![-1i32; num_iter];
    for (i, &n) in nums.iter().enumerate().take(nums.len() - 1) {
        memo[n as usize] = i as i32;
    }

    let mut last_spoken_val = *nums.last().unwrap();
    for i in nums.len()..num_iter {
        let prev_idx = memo[last_spoken_val as usize];
        memo[last_spoken_val as usize] = (i - 1) as i32;
        last_spoken_val = if prev_idx == -1 {
            0
        } else {
            (i - 1) as i32 - prev_idx
        }
    }

    let answer = last_spoken_val;
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
    let nums: Vec<i32> = fs::read_to_string(file_path)
        .unwrap()
        .trim()
        .split(',')
        .map(|d| d.parse().unwrap())
        .collect();

    let num_iter = 30000000;
    let mut memo = vec![-1i32; num_iter];
    for (i, &n) in nums.iter().enumerate().take(nums.len() - 1) {
        memo[n as usize] = i as i32;
    }

    let mut last_spoken_val = *nums.last().unwrap();
    for i in nums.len()..num_iter {
        let prev_idx = memo[last_spoken_val as usize];
        memo[last_spoken_val as usize] = (i - 1) as i32;
        last_spoken_val = if prev_idx == -1 {
            0
        } else {
            (i - 1) as i32 - prev_idx
        }
    }

    let answer = last_spoken_val;

    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
