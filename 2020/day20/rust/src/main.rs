use std::fs;
use std::io::{self, BufRead};
use std::path::Path;
use std::time::Instant;

fn main() {
    let test_path: &str = "src/test.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test ");
    part_one(test_path);
    // print!("Input ");
    // part_one(input_path);

    println!("========= PART TWO ==========");
    // print!("Test ");
    // part_two(test_path);
    // print!("Input ");
    // part_two(input_path);
}

// Used to read text file line by line

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem

    let mut answer: Option<u64> = None;
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer.unwrap_or(0),
        elapsed.as_secs_f64() * 1000.0
    );
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();

    // Parse the problem

    let mut answer: Option<u64> = None;
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer.unwrap_or(0),
        elapsed.as_secs_f64() * 1000.0
    );
}
