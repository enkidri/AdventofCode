use std::collections::HashMap;
use std::fs;
use std::time::Instant;

fn main() {
    let test1_path: &str = "src/test1.txt";
    let test2_path: &str = "src/test2.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test 1 ");
    part_one(test1_path);
    print!("Test 2 ");
    part_one(test2_path);
    print!("Input ");
    part_one(input_path);

    println!("========= PART TWO ==========");
    print!("Test 1 ");
    part_two(test1_path);
    print!("Test 2 ");
    part_two(test2_path);
    print!("Input ");
    part_two(input_path);
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();

    // Parse the problem
    let mut joltage_ratings: Vec<u64> = fs::read_to_string(file_path)
        .unwrap()
        .lines()
        .map(|l| l.parse().unwrap())
        .collect();
    joltage_ratings.push(0);

    // solve
    joltage_ratings.sort_unstable();
    joltage_ratings.push(joltage_ratings.last().unwrap() + 3u64);

    let map: HashMap<u64, u64> = joltage_ratings.windows(2).fold(HashMap::new(), |mut m, w| {
        let diff = w[1] - w[0];
        *m.entry(diff).or_insert(0) += 1;
        m
    });

    let answer = Some(map[&1u64] * map[&3u64]);

    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer.unwrap_or(0),
        elapsed.as_secs_f64() * 1000.0
    );
}

fn dfs(
    current_idx: usize,
    goal_idx: usize,
    joltages: &[u64],
    memo: &mut HashMap<usize, u64>,
) -> u64 {
    if current_idx == goal_idx {
        return 1u64;
    }
    if let Some(&cached) = memo.get(&current_idx) {
        return cached;
    }

    let current_val = joltages[current_idx];

    // Use an iterator to sum paths
    let total_paths = (current_idx + 1..joltages.len())
        .take_while(|&i| joltages[i] <= current_val + 3)
        .map(|i| dfs(i, goal_idx, joltages, memo))
        .sum();

    memo.insert(current_idx, total_paths);
    total_paths
}

fn compute_num_arrangement(joltage_ratings: &Vec<u64>) -> u64 {
    let goal_idx = joltage_ratings.len() - 1;
    let mut memo: HashMap<usize, u64> = HashMap::new();
    dfs(0, goal_idx, joltage_ratings, &mut memo)
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();

    // Parse the problem
    let mut joltage_ratings: Vec<u64> = fs::read_to_string(file_path)
        .unwrap()
        .lines()
        .map(|l| l.parse().unwrap())
        .collect();

    // solve
    joltage_ratings.push(0);
    joltage_ratings.sort_unstable();
    joltage_ratings.push(joltage_ratings.last().unwrap() + 3u64);

    let answer: Option<u64> = Some(compute_num_arrangement(&joltage_ratings));
    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer.unwrap_or(0),
        elapsed.as_secs_f64() * 1000.0
    );
}
