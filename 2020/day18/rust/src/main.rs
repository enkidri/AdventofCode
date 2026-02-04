use std::fs;
use std::io::{self, BufRead};
use std::path::Path;
use std::time::Instant;

fn main() {
    let test_path: &str = "src/test.txt";
    let test1_path: &str = "src/test1.txt";
    let test2_path: &str = "src/test2.txt";
    let test3_path: &str = "src/test3.txt";
    let test4_path: &str = "src/test4.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test ");
    part_one(test_path);
    part_one(test1_path);
    part_one(test2_path);
    part_one(test3_path);
    part_one(test4_path);
    print!("Input ");
    part_one(input_path);

    println!("========= PART TWO ==========");
    print!("Test ");
    part_two(test_path);
    part_two(test1_path);
    part_two(test2_path);
    part_two(test3_path);
    part_two(test4_path);
    print!("Input ");
    part_two(input_path);
}

fn do_op(num: &mut Vec<u64>, ops: &mut Vec<char>) {
    let op = ops.pop().unwrap();
    let right = num.pop().unwrap();
    let left = num.pop().unwrap();
    match op {
        '*' => num.push(left * right),
        '+' => num.push(left + right),
        _ => panic!("Unsupprtoed operator {}", op),
    }
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem
    let content = fs::read_to_string(file_path).expect("Couldnt read file");
    let answer: u64 = content
        .lines()
        .map(|line| {
            let mut num: Vec<u64> = Vec::new();
            let mut ops: Vec<char> = Vec::new();
            for c in line.chars() {
                match c {
                    '0'..='9' => {
                        num.push(c.to_digit(10).unwrap() as u64);
                    }
                    '(' => {
                        ops.push(c);
                    }
                    ')' => {
                        while let Some(&top_op) = ops.last() {
                            if top_op == '(' {
                                ops.pop();
                                break;
                            }
                            do_op(&mut num, &mut ops);
                        }
                    }
                    '+' | '*' => {
                        while !ops.is_empty() && !matches!(ops.last(), Some('(')) {
                            do_op(&mut num, &mut ops);
                        }
                        ops.push(c);
                    }
                    _ => {}
                }
            }
            while !ops.is_empty() {
                do_op(&mut num, &mut ops);
            }
            num.last().unwrap().clone()
        })
        .sum();

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
    let content = fs::read_to_string(file_path).expect("Couldnt read file");
    let answer: u64 = content
        .lines()
        .map(|line| {
            let mut num: Vec<u64> = Vec::new();
            let mut ops: Vec<char> = Vec::new();
            for c in line.chars() {
                match c {
                    '0'..='9' => {
                        num.push(c.to_digit(10).unwrap() as u64);
                    }
                    '(' => {
                        ops.push(c);
                    }
                    ')' => {
                        while let Some(&top_op) = ops.last() {
                            if top_op == '(' {
                                ops.pop();
                                break;
                            }
                            do_op(&mut num, &mut ops);
                        }
                    }
                    '+' | '*' => {
                        while !ops.is_empty()
                            && !matches!(ops.last(), Some('('))
                            && matches!(ops.last(), Some('+'))
                        {
                            do_op(&mut num, &mut ops);
                        }
                        ops.push(c);
                    }
                    _ => {}
                }
            }
            while !ops.is_empty() {
                do_op(&mut num, &mut ops);
            }
            num.last().unwrap().clone()
        })
        .sum();

    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
