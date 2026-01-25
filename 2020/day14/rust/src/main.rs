use std::collections::HashMap;
use std::fs;
use std::time::Instant;

fn main() {
    let test_path: &str = "src/test.txt";
    let test1_path: &str = "src/test1.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test ");
    part_one(test_path);
    print!("Input ");
    part_one(input_path);

    println!("========= PART TWO ==========");
    print!("Test ");
    part_two(test1_path);
    print!("Input ");
    part_two(input_path);
}

// Used to read text file line by line

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem
    let lines = fs::read_to_string(file_path).expect("Couldnt read file");

    let mut mem: HashMap<u64, u64> = HashMap::new();
    let mut mask_and = 0u64;
    let mut mask_or = 0u64;

    for line in lines.lines() {
        if line.starts_with("mask") {
            // reset mask
            mask_and = 0u64;
            mask_or = 0u64;
            let mask_str = &line[7..];
            for c in mask_str.chars() {
                mask_and <<= 1;
                mask_or <<= 1;
                match c {
                    'X' => mask_and |= 1,
                    '1' => mask_or |= 1,
                    _ => (),
                }
            }
        } else {
            let parts: Vec<&str> = line
                .split(|c: char| !c.is_numeric())
                .filter(|s| !s.is_empty())
                .collect();

            if let [addr, val] = parts.as_slice() {
                let addr: u64 = addr.parse().unwrap();
                let val: u64 = val.parse().unwrap();

                let result = (val & mask_and) | mask_or;
                mem.insert(addr, result);
            }
        }
    }

    let answer: u64 = mem.values().sum();

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
    let lines = fs::read_to_string(file_path).expect("Couldnt read file");

    let mut mem: HashMap<u64, u64> = HashMap::new();
    let mut x_indices: Vec<u64> = Vec::new();
    let mut mask_or = 0u64;

    for line in lines.lines() {
        if line.starts_with("mask") {
            // reset mask
            mask_or = 0u64;
            x_indices.clear();
            let mask_str = &line[7..];
            for (i, c) in mask_str.chars().enumerate() {
                mask_or <<= 1;
                match c {
                    'X' => x_indices.push((mask_str.len() - 1 - i) as u64),
                    '1' => mask_or |= 1,
                    _ => (),
                }
            }
        } else {
            let parts: Vec<&str> = line
                .split(|c: char| !c.is_numeric())
                .filter(|s| !s.is_empty())
                .collect();

            if let [addr, val] = parts.as_slice() {
                let addr: u64 = addr.parse().unwrap();
                let val: u64 = val.parse().unwrap();

                // Apply rule 1 and 2
                let res = addr | mask_or;

                let n = x_indices.len();
                for p in 0..(1 << n) {
                    let mut new_addr = res;
                    for i in 0..n {
                        let bit = (p >> i) & 1;

                        if bit == 1 {
                            new_addr |= 1 << x_indices[i];
                        } else {
                            new_addr &= !(1 << x_indices[i]);
                        }
                    }

                    mem.insert(new_addr, val);
                }
            }
        }
    }

    let answer: u64 = mem.values().sum();

    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
