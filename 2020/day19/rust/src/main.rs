use rayon::prelude::*;
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

#[derive(Debug)]
#[warn(dead_code)]
struct Rule {
    next: Option<Vec<Vec<i32>>>,
    c: Option<char>,
}

fn dfs(
    state: (i32, i32),
    message: &str,
    rules: &HashMap<i32, Rule>,
    memo: &mut HashMap<(i32, i32), Vec<i32>>,
) -> Vec<i32> {
    // check cache
    let (current_idx, rule_id) = state;
    if let Some(cache) = memo.get(&state) {
        return cache.clone();
    }

    // get rule
    let Some(rule) = rules.get(&rule_id) else {
        return vec![];
    };

    let results: Vec<i32> = if let Some(c) = rule.c {
        if message.chars().nth(current_idx as usize) == Some(c) {
            vec![current_idx + 1]
        } else {
            vec![]
        }
    } else if let Some(sequences) = &rule.next {
        let mut results = Vec::new();
        for sequence in sequences {
            let mut current_positions: Vec<i32> = vec![current_idx];
            for &sub_rule in sequence {
                let mut next_positions = Vec::new();
                for &pos in &current_positions {
                    let new_starts = dfs((pos, sub_rule), message, rules, memo);
                    next_positions.extend(new_starts);
                }
                next_positions.sort_unstable();
                next_positions.dedup();
                current_positions = next_positions;
                if current_positions.is_empty() {
                    break;
                }
            }
            results.extend(current_positions);
        }
        results
    } else {
        vec![]
    };

    // update cache
    memo.insert(state, results.clone());
    results
}

fn compute_valid_message(message: &str, rules: &HashMap<i32, Rule>) -> bool {
    let mut memo: HashMap<(i32, i32), Vec<i32>> = HashMap::new();
    let result_indices = dfs((0, 0), message, rules, &mut memo);
    result_indices
        .iter()
        .any(|&val| val == message.len() as i32)
}

// Used to read text file line by line
#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();
    // Parse the problem
    let content = fs::read_to_string(file_path).expect("not found");
    let groups: Vec<&str> = content.split("\n\n").collect();
    let rules: HashMap<i32, Rule> = groups[0].lines().fold(HashMap::new(), |mut m, l| {
        let sections: Vec<&str> = l.split(":").collect();
        let id: i32 = sections[0].parse().unwrap();
        if sections[1].contains('"') {
            let c = sections[1]
                .chars()
                .nth(sections[1].find('"').unwrap() + 1)
                .unwrap();
            m.insert(
                id,
                Rule {
                    next: None,
                    c: Some(c),
                },
            );
        } else {
            let next: Vec<Vec<i32>> = sections[1]
                .split('|')
                .map(|l| {
                    l.split_whitespace()
                        .map(|num| num.parse().unwrap())
                        .collect()
                })
                .collect();
            m.insert(
                id,
                Rule {
                    next: Some(next),
                    c: None,
                },
            );
        }
        m
    });
    let messages: Vec<&str> = groups[1].lines().collect();

    // solve
    let answer: u64 = messages
        .par_iter()
        .filter(|&message| compute_valid_message(message, &rules))
        .count()
        .try_into()
        .unwrap();

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
    let content = fs::read_to_string(file_path).expect("not found");
    let groups: Vec<&str> = content.split("\n\n").collect();
    let mut rules: HashMap<i32, Rule> = groups[0].lines().fold(HashMap::new(), |mut m, l| {
        let sections: Vec<&str> = l.split(":").collect();
        let id: i32 = sections[0].parse().unwrap();
        if sections[1].contains('"') {
            let c = sections[1]
                .chars()
                .nth(sections[1].find('"').unwrap() + 1)
                .unwrap();
            m.insert(
                id,
                Rule {
                    next: None,
                    c: Some(c),
                },
            );
        } else {
            let next: Vec<Vec<i32>> = sections[1]
                .split('|')
                .map(|l| {
                    l.split_whitespace()
                        .map(|num| num.parse().unwrap())
                        .collect()
                })
                .collect();
            m.insert(
                id,
                Rule {
                    next: Some(next),
                    c: None,
                },
            );
        }
        m
    });
    rules.insert(
        8,
        Rule {
            c: None,
            next: Some(vec![vec![42], vec![42, 8]]),
        },
    );
    rules.insert(
        11,
        Rule {
            c: None,
            next: Some(vec![vec![42, 31], vec![42, 11, 31]]),
        },
    );
    let messages: Vec<&str> = groups[1].lines().collect();

    // solve
    let answer: u64 = messages
        .par_iter()
        .filter(|&message| compute_valid_message(message, &rules))
        .count()
        .try_into()
        .unwrap();

    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
