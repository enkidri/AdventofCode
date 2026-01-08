use std::collections::{HashMap, HashSet, VecDeque};
use std::fs;
use std::io::{self, BufRead};
use std::path::Path;
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
    part_two(test_path); // Should return 32
    print!("Test 1 "); // Should return 126
    part_two(test1_path);
    print!("Input ");
    part_two(input_path);
}

#[derive(Debug)]
struct BagContent {
    content: String,
    count: u64,
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();

    let mut answer: u64 = 0;
    // Parse the problem
    let content = fs::read_to_string(file_path).expect("Couldnt read file");
    let lines: Vec<&str> = content.lines().collect();
    let map: HashMap<String, Vec<BagContent>> = lines.iter().fold(HashMap::new(), |mut m, line| {
        let words: Vec<&str> = line.split_whitespace().collect();
        let key = format!("{} {}", words[0], words[1]);
        let item: Vec<BagContent> = words[4..]
            .chunks(4)
            .filter_map(|chunk| {
                if chunk[0] == "no" {
                    return None;
                }

                Some(BagContent {
                    count: chunk[0].parse().unwrap_or(0),
                    content: format!("{} {}", chunk[1], chunk[2]),
                })
            })
            .collect();
        m.insert(key, item);
        m
    });

    // Solve by reversing
    let map_reversed: HashMap<String, Vec<BagContent>> =
        map.into_iter().fold(HashMap::new(), |mut m, (k, v)| {
            for val in v {
                m.entry(val.content).or_default().push(BagContent {
                    content: k.clone(),
                    count: val.count,
                });
            }
            m
        });

    let target_bag = "shiny gold";
    let mut visited: HashSet<String> = HashSet::new();
    let mut q = VecDeque::new();
    q.push_back(target_bag.to_string());
    while let Some(bag) = q.pop_front() {
        if let Some(parents) = map_reversed.get(&bag) {
            for parent in parents {
                if visited.insert(parent.content.clone()) {
                    q.push_back(parent.content.clone());
                }
            }
        }
    }

    answer += visited.len() as u64;

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

fn compute_num_bags(start: &str, map: &HashMap<String, Vec<BagContent>>) -> u64 {
    let mut memo: HashMap<String, u64> = HashMap::new();
    dfs(start, map, &mut memo)
}

fn dfs(
    current: &str,
    map: &HashMap<String, Vec<BagContent>>,
    memo: &mut HashMap<String, u64>,
) -> u64 {
    // Check memoization
    if let Some(&total) = memo.get(current) {
        return total;
    }

    let mut total = 0;
    if let Some(contents) = map.get(current) {
        for item in contents {
            let bag_count = dfs(&item.content, map, memo);
            total += item.count + (item.count * bag_count);
        }
    }
    memo.insert(current.to_string(), total);

    total
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();

    let mut answer: u64 = 0;
    // Parse the problem
    let content = fs::read_to_string(file_path).expect("Couldnt read file");
    let map: HashMap<String, Vec<BagContent>> =
        content.lines().fold(HashMap::new(), |mut m, line| {
            let words: Vec<&str> = line.split_whitespace().collect();
            let key = format!("{} {}", words[0], words[1]);
            let item: Vec<BagContent> = words[4..]
                .chunks(4)
                .filter_map(|chunk| {
                    if chunk[0] == "no" {
                        return None;
                    }

                    Some(BagContent {
                        count: chunk[0].parse().unwrap_or(0),
                        content: format!("{} {}", chunk[1], chunk[2]),
                    })
                })
                .collect();
            m.insert(key, item);
            m
        });

    answer += compute_num_bags("shiny gold", &map);

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
