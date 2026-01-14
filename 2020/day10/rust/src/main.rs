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
    print!("Input ");
    part_one(input_path);

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

    let mut answer: Option<u64> = None;
    // Parse the problem
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {}
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();

    let mut answer: Option<u64> = None;
    // Parse the problem
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {}
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
