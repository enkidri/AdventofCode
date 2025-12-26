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
    print!("Test ");
    part_two(test_path);
    print!("Input ");
    part_two(input_path);
}

#[derive(Debug, Copy, Clone)]
struct Index {
    i : usize,
    j : usize,
}

impl Add for Index {
    type Output = Self;

    fn add(self, other: Self) -> Self {
        Self {
            i : self.i + other.i,
            j : self.j + other.j,
        }
    }
}

// Used to read text file line by line
fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<fs::File>>>
where P: AsRef<Path>, {
    let file = fs::File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn part_one(file_path : &str) {
    let now = Instant::now();
    let mut answer : u64 = 0;

    // Parse the problem
    let mut map : Vec<String> = Vec::new();
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            map.push(line);
        }
    }

    let n : usize = map.len();
    let m : usize = map[0].len();
    let mut current : Index = Index{i : 0, j : 0};
    let to_move : Index = Index{i : 1, j : 3};
    for _ in 0..(n-1) / to_move.i {
        let next = current + to_move;
        current = next;
        if map[current.i].chars().nth(current.j % m) == Some('#') {
            answer += 1;
        }
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

#[allow(dead_code)]
fn part_two(file_path : &str) {
    let now = Instant::now();
    let mut answer : u64 = 1;

    // Parse the problem
    let mut map : Vec<String> = Vec::new();
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            map.push(line);
        }
    }

    // Solve
    let n : usize = map.len();
    let m : usize = map[0].len();
    let mut current : Index = Index{i : 0, j : 0};
    let to_move_vec = vec![
        Index{i : 1, j : 1},
        Index{i : 1, j : 3},
        Index{i : 1, j : 5},
        Index{i : 1, j : 7},
        Index{i : 2, j : 1},
    ];
    for to_move in to_move_vec.iter() {
        let mut count : u64 = 0;
        for _ in 0..(n-1) / to_move.i {
            let next = current + *to_move;
            current = next;
            if map[current.i].chars().nth(current.j % m) == Some('#') {
                count += 1;
            }
        }
        current = Index{i : 0, j : 0};
        answer *= count;
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
