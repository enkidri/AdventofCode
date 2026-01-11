use std::collections::HashSet;
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

#[allow(dead_code)]
#[derive(Debug)]
struct Instruction {
    op: String,
    arg: i64,
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();

    let mut answer: u64 = 0;
    // Parse the problem
    let instructions: Vec<Instruction> = fs::read_to_string(file_path)
        .expect("Couldnt read file")
        .lines()
        .map(|l| {
            let mut words = l.split_whitespace();
            Instruction {
                op: words.next().unwrap().to_string(),
                arg: words.next().unwrap().parse().unwrap(),
            }
        })
        .collect();

    // Solve
    let mut global_value = 0;
    let mut to_visit: usize = 0;
    let mut visited: HashSet<usize> = HashSet::new();
    while !visited.contains(&to_visit) {
        visited.insert(to_visit);

        let instruction = &instructions[to_visit];
        if instruction.op == "nop" {
            to_visit += 1;
        } else if instruction.op == "jmp" {
            to_visit = (to_visit as i64 + instruction.arg) as usize;
        } else if instruction.op == "acc" {
            to_visit += 1;
            global_value += instruction.arg;
        }
    }
    answer += global_value as u64;

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

struct Result {
    terminated: bool,
    value: u64,
}

fn run(
    mut to_visit: usize,
    goal_idx: usize,
    mut global_value: i64,
    mut visited: HashSet<usize>,
    instructions: &Vec<Instruction>,
) -> Result {
    while to_visit > 0 && to_visit < goal_idx {
        if visited.contains(&to_visit) {
            return Result {
                terminated: false,
                value: 0,
            };
        }
        visited.insert(to_visit);

        let instruction = &instructions[to_visit];
        if instruction.op == "nop" {
            to_visit += 1;
        } else if instruction.op == "jmp" {
            to_visit = (to_visit as i64 + instruction.arg) as usize;
        } else if instruction.op == "acc" {
            to_visit += 1;
            global_value += instruction.arg;
        }
    }
    Result {
        terminated: to_visit == goal_idx,
        value: global_value as u64,
    }
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();

    let mut answer: u64 = 0;
    // Parse the problem
    let instructions: Vec<Instruction> = fs::read_to_string(file_path)
        .expect("Couldnt read file")
        .lines()
        .map(|l| {
            let mut words = l.split_whitespace();
            Instruction {
                op: words.next().unwrap().to_string(),
                arg: words.next().unwrap().parse().unwrap(),
            }
        })
        .collect();

    // Solve

    let goal_idx: usize = instructions.len();
    let mut global_value = 0;
    let mut to_visit: usize = 0;
    let mut visited: HashSet<usize> = HashSet::new();
    while !visited.contains(&to_visit) {
        visited.insert(to_visit);

        let instruction = &instructions[to_visit];
        // branch out
        if instruction.op == "jmp" || instruction.op == "nop" {
            let alt_visit = if instruction.op == "jmp" {
                (to_visit + 1) as usize
            } else {
                (to_visit as i64 + instruction.arg) as usize
            };
            let result = run(
                alt_visit,
                goal_idx,
                global_value,
                visited.clone(),
                &instructions,
            );
            if result.terminated {
                answer = result.value;
                break;
            }
        }

        if instruction.op == "nop" {
            to_visit += 1;
        } else if instruction.op == "jmp" {
            to_visit = (to_visit as i64 + instruction.arg) as usize;
        } else if instruction.op == "acc" {
            to_visit += 1;
            global_value += instruction.arg;
        }
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
