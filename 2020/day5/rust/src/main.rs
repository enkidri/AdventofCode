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
    part_one(test_path);
    print!("Input ");
    part_one(input_path);

    println!("========= PART TWO ==========");
    // print!("Test ");
    // part_two(test_path); // No test for this day
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

fn binary_partition(input: &str) -> u64 {
    input.chars().fold(0, |acc, c| {
        // Shift bits left and add 1 if it's the "upper" character
        (acc << 1) | (if c == 'B' || c == 'R' { 1 } else { 0 })
    })
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    let mut answer: u64 = 0;
    // Parse the problem
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            let row_str = &line[0..7];
            let col_str = &line[7..];

            let row_nr = binary_partition(row_str);
            let col_nr = binary_partition(col_str);
            answer = answer.max(row_nr * 8 + col_nr);
        }
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();
    let mut answer: u64 = 0;
    // Parse the problem
    let mut occupation_idx = vec![false; 1024];
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            let row_str = &line[0..7];
            let col_str = &line[7..];

            let row_nr = binary_partition(row_str);
            let col_nr = binary_partition(col_str);
            occupation_idx[(row_nr * 8 + col_nr) as usize] = true;
        }
    }

    let idx = occupation_idx
        .windows(3)
        .position(|w| matches!(w, [true, false, true]));
    answer += idx.unwrap_or(0) as u64 + 1;

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
