use std::fs;
use std::io::{self, BufRead};
use std::path::Path;
use std::time::Instant;
use std::ops::Add;

fn main() {
    let test_path : &str = "src/test.txt";
    let input_path : &str = "src/input.txt";
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
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<fs::File>>>
where P: AsRef<Path>, {
    let file = fs::File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

#[allow(dead_code)]
fn part_one(file_path : &str) {
    let now = Instant::now();
    // Parse the problem
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
        }
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

#[allow(dead_code)]
fn part_two(file_path : &str) {
    let now = Instant::now();
    // Parse the problem
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
        }
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
