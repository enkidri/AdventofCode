use std::fs;
use std::io::{self, BufRead};
use std::path::Path;
use std::time::Instant;
use regex::Regex;

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
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<fs::File>>>
where
    P: AsRef<Path>,
{
    let file = fs::File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
fn part_one(file_path: &str) {
    let now = Instant::now();
    let mut answer: i64 = 0;

    // Parse the problem
    let re = Regex::new(r"(\d+)-(\d+) (\w): (.*)").unwrap();
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            let cap = re.captures(&line).expect("Line format invalid");
            let min : usize = cap[1].parse().unwrap();
            let max : usize = cap[2].parse().unwrap();
            let target : char = cap[3].chars().next().unwrap();
            let password = &cap[4];

            let target_count = password.bytes().filter(|&b| b == target as u8).count();

            if (min..=max).contains(&target_count) {
                answer += 1;
            }
        }
    }

    let elapsed = now.elapsed();
    println!("answer: {} Time: {} ms", answer, elapsed.as_millis());
}

fn part_two(file_path: &str) {
    let now = Instant::now();
    let mut answer: i64 = 0;
    // Parse the problem
    let re = Regex::new(r"(\d+)-(\d+) (\w): (.*)").unwrap();
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            let cap = re.captures(&line).expect("Line format invalid");
            let lower : usize = cap[1].parse().unwrap();
            let top : usize = cap[2].parse().unwrap();
            let target : char = cap[3].chars().next().unwrap();
            let password = &cap[4];

            // Solve
            let mut lower_char : char = '0';
            let mut upper_char : char = '0';
            for (i, c) in password.chars().enumerate() {
                if i + 1 == lower as usize {
                    lower_char = c;
                } else if i + 1 == top as usize {
                    upper_char = c;
                };
            }

            answer += ((lower_char == target) != (upper_char == target)) as i64;
        }
    }

    let elapsed = now.elapsed();
    println!("answer: {} Time: {} ms", answer, elapsed.as_millis());
}
