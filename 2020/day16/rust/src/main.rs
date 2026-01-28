use regex::Regex;
use std::fs;
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
    // part_two(test_path);
    print!("Input ");
    part_two(input_path);
}

// Used to read text file line by line
struct Range {
    lower: i32,
    upper: i32,
}

struct ClassRange {
    name: String,
    a: Range,
    b: Range,
}

#[allow(dead_code)]
fn part_one(file_path: &str) {
    let now = Instant::now();

    let lines = fs::read_to_string(file_path).expect("File not found");
    let sections: Vec<&str> = lines.split("\n\n").collect();
    let rules_section = sections[0];
    let nearby_tickets_section = sections[2];

    let re = Regex::new(r"([^:]+): (\d+)-(\d+) or (\d+)-(\d+)").unwrap();

    // Parse the ticket rules
    let mut rules: Vec<ClassRange> = Vec::new();
    for line in rules_section.lines() {
        if let Some(cap) = re.captures(line) {
            rules.push(ClassRange {
                name: cap[1].to_string(),
                a: Range {
                    lower: cap[2].parse().unwrap(),
                    upper: cap[3].parse().unwrap(),
                },
                b: Range {
                    lower: cap[4].parse().unwrap(),
                    upper: cap[5].parse().unwrap(),
                },
            })
        }
    }

    // Parse the nearby tickets
    let nearby_tickets: Vec<Vec<i32>> = nearby_tickets_section
        .lines()
        .skip(1)
        .map(|l| l.split(',').map(|c| c.parse().unwrap()).collect())
        .collect();

    let is_valid = |val: &i32| {
        rules.iter().any(|rule| {
            *val >= rule.a.lower && *val <= rule.a.upper
                || *val >= rule.b.lower && *val <= rule.b.upper
        })
    };

    let ticket_error_sum: i32 = nearby_tickets
        .iter()
        .flatten()
        .filter(|&&val| !is_valid(&val))
        .sum();

    let answer = ticket_error_sum;
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

    let lines = fs::read_to_string(file_path).expect("File not found");
    let sections: Vec<&str> = lines.split("\n\n").collect();
    let rules_section = sections[0];
    let my_ticket_section = sections[1];
    let nearby_tickets_section = sections[2];

    let re = Regex::new(r"([^:]+): (\d+)-(\d+) or (\d+)-(\d+)").unwrap();

    // Parse the ticket rules
    let mut rules: Vec<ClassRange> = Vec::new();
    for line in rules_section.lines() {
        if let Some(cap) = re.captures(line) {
            rules.push(ClassRange {
                name: cap[1].to_string(),
                a: Range {
                    lower: cap[2].parse().unwrap(),
                    upper: cap[3].parse().unwrap(),
                },
                b: Range {
                    lower: cap[4].parse().unwrap(),
                    upper: cap[5].parse().unwrap(),
                },
            })
        }
    }

    // Parse my tickets
    let my_tickets: Vec<u64> = my_ticket_section
        .lines()
        .skip(1)
        .next()
        .map(|l| l.split(',').map(|c| c.parse().unwrap()).collect())
        .unwrap();

    // Parse the nearby tickets
    let nearby_tickets: Vec<Vec<i32>> = nearby_tickets_section
        .lines()
        .skip(1)
        .map(|l| l.split(',').map(|c| c.parse().unwrap()).collect())
        .collect();

    // Solve
    let is_valid = |val: &i32| {
        rules.iter().any(|rule| {
            *val >= rule.a.lower && *val <= rule.a.upper
                || *val >= rule.b.lower && *val <= rule.b.upper
        })
    };
    let valid_nearby_ticket: Vec<&Vec<i32>> = nearby_tickets
        .iter()
        .filter(|&l| l.iter().all(|v| is_valid(v)))
        .collect();

    // Generate potential ranges
    let full_mask: u32 = (1 << rules.len()) - 1;
    let mut col_masks = vec![full_mask; rules.len()];
    for &ticket in &valid_nearby_ticket {
        for (j, &val) in ticket.iter().enumerate() {
            let mut possible_range: u32 = 0;
            for (rule_idx, rule) in rules.iter().enumerate() {
                if (val >= rule.a.lower && val <= rule.a.upper)
                    || (val >= rule.b.lower && val <= rule.b.upper)
                {
                    possible_range |= 1 << rule_idx;
                }
            }
            col_masks[j] &= possible_range;
        }
    }

    let mut solved_mask = 0;
    while solved_mask != full_mask {
        for i in 0..rules.len() {
            if col_masks[i].count_ones() == 1 && (col_masks[i] & solved_mask) == 0 {
                let current_bit = col_masks[i];
                solved_mask |= current_bit;

                for j in 0..col_masks.len() {
                    if i != j {
                        col_masks[j] &= !current_bit;
                    }
                }
            }
        }
    }

    let mut answer: u64 = 1;
    for (col_idx, &mask) in col_masks.iter().enumerate() {
        let idx = mask.trailing_zeros() as usize;
        let rule = &rules[idx];
        let rule_name = &rule.name;

        if rule_name.starts_with("departure") {
            answer *= my_tickets[col_idx];
        }
    }

    let elapsed = now.elapsed();
    println!(
        "Answer: {} Time: {:.3} ms",
        answer,
        elapsed.as_secs_f64() * 1000.0
    );
}
