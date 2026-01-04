use std::collections::HashMap;
use std::collections::HashSet;
use std::fs;
use std::io::{self, BufRead};
use std::ops::Add;
use std::path::Path;
use std::time::Instant;

type Group = Vec<String>;

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
fn part_one(file_path: &str) {
    let now = Instant::now();

    let mut answer: u64 = 0;
    // Parse the problem
    let mut groups: Vec<Group> = Vec::new();
    let mut group: Group = Vec::new();
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            if line.is_empty() {
                groups.push(group);
                group = Vec::new();
                continue;
            }

            group.push(line);
        }
    }
    groups.push(group);

    // Solve
    answer += groups
        .iter()
        .map(|group| {
            group
                .iter()
                .flat_map(|c| c.chars())
                .collect::<HashSet<_>>()
                .len() as u64
        })
        .sum::<u64>();

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();

    let mut answer: u64 = 0;
    // Parse the problem
    let content = fs::read_to_string(file_path).expect("Failed to read file.");
    let groups: Vec<Vec<String>> = content
        .split("\n\n")
        .map(|block| block.lines().map(|s| s.to_string()).collect())
        .collect();

    // Solve
    answer += groups
        .iter()
        .map(|group| {
            let counts = group
                .iter()
                .flat_map(|s| s.chars())
                .fold(HashMap::new(), |mut map, c| {
                    *map.entry(c).or_insert(0) += 1;
                    map
                });
            counts.iter().filter(|&(_k, v)| *v == group.len()).count() as u64
        })
        .sum::<u64>();

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
