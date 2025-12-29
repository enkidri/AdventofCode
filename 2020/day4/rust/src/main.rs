use regex::Regex;
use std::collections::HashMap;
use std::fs;
use std::io::{self, BufRead};
use std::ops::Add;
use std::path::Path;
use std::time::Instant;

fn main() {
    let test_path: &str = "src/test.txt";
    let test1_path: &str = "src/test1.txt";
    let test2_path: &str = "src/test2.txt";
    let input_path: &str = "src/input.txt";
    println!("========= PART ONE ==========");
    print!("Test ");
    part_one(test_path);
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
    let re = Regex::new(r"(([^\s]+):([^\s]+))").unwrap();
    let mut maps: Vec<HashMap<String, String>> = Vec::new();
    let mut map: HashMap<String, String> = HashMap::new();
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            if line.is_empty() {
                maps.push(map);
                map = HashMap::new();
                continue;
            }

            for cap in re.captures_iter(&line) {
                let key = &cap[2];
                let value = &cap[3];
                map.insert(key.to_owned(), value.to_owned());
            }
        }
    }
    maps.push(map);

    // Solve
    let required_keys = vec!["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"];
    for map in maps.iter() {
        let mut required_keys_present = true;
        for key in required_keys.iter() {
            if !map.contains_key(*key) {
                required_keys_present = false;
                break;
            }
        }

        if required_keys_present {
            answer += 1;
        }
    }

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}

// Perhaps I can remove the two first conditions as they are redundant?
fn is_ok_byr(input: &str) -> bool {
    input.len() == 4
        && input.chars().all(|c| c.is_ascii_digit())
        && input
            .parse::<i32>()
            .map_or(false, |val| val >= 1920 && val <= 2002)
}

fn is_ok_iyr(input: &str) -> bool {
    input.len() == 4
        && input.chars().all(|c| c.is_ascii_digit())
        && input
            .parse::<i32>()
            .map_or(false, |val| val >= 2010 && val <= 2020)
}

fn is_ok_eyr(input: &str) -> bool {
    input.len() == 4
        && input.chars().all(|c| c.is_ascii_digit())
        && input
            .parse::<i32>()
            .map_or(false, |val| val >= 2020 && val <= 2030)
}

fn is_ok_hgt(input: &str) -> bool {
    if input.len() < 4 {
        return false;
    }

    let (number_str, unit) = input.split_at(input.len() - 2);

    let Ok(number) = number_str.parse::<i32>() else {
        return false;
    };

    match unit {
        "cm" => (150..=193).contains(&number),
        "in" => (59..=76).contains(&number),
        _ => false,
    }
}

fn is_ok_hcl(input: &str) -> bool {
    if !input.starts_with('#') && input.len() != 7 {
        return false;
    }

    input[1..]
        .chars()
        .all(|c| ('0'..='9').contains(&c) || ('a'..='f').contains(&c))
}

fn is_ok_ecl(input: &str) -> bool {
    let to_match = vec!["amb", "blu", "brn", "gry", "grn", "hzl", "oth"];
    to_match.contains(&input)
}
fn is_ok_pid(input: &str) -> bool {
    (input.len() == 9) && (input.chars().all(|c| c.is_ascii_digit()))
}

fn required_keys_is_present(map: &HashMap<String, String>) -> bool {
    static REQUIRED_KEYS: [&str; 7] =
        ["byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"];
    REQUIRED_KEYS.iter().all(|key| map.contains_key(*key))
}

#[allow(dead_code)]
fn part_two(file_path: &str) {
    let now = Instant::now();
    let mut answer: u64 = 0;

    // Parse the problem
    let re = Regex::new(r"(([^\s]+):([^\s]+))").unwrap();
    let mut maps: Vec<HashMap<String, String>> = Vec::new();
    let mut map: HashMap<String, String> = HashMap::new();
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.map_while(Result::ok) {
            if line.is_empty() {
                maps.push(map);
                map = HashMap::new();
                continue;
            }

            for cap in re.captures_iter(&line) {
                let key = &cap[2];
                let value = &cap[3];
                map.insert(key.to_owned(), value.to_owned());
            }
        }
    }
    maps.push(map);

    // Solve
    answer += maps
        .iter()
        .filter(|map| {
            required_keys_is_present(map)
                && map.iter().all(|(k, v)| match k.as_str() {
                    "byr" => is_ok_byr(v),
                    "iyr" => is_ok_iyr(v),
                    "eyr" => is_ok_eyr(v),
                    "hgt" => is_ok_hgt(v),
                    "hcl" => is_ok_hcl(v),
                    "ecl" => is_ok_ecl(v),
                    "pid" => is_ok_pid(v),
                    _ => true,
                })
        })
        .count() as u64;

    let elapsed = now.elapsed();
    println!("Answer: {} Time: {} ms", answer, elapsed.as_millis());
}
