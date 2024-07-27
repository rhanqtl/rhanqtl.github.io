use std::functional::map

def main()
    let nums /* : List<int> */ = [1, 2, 3, 4, 5];
    let evens = nums.filter(x -> x % 2 == 0).collect()
    let sum_of_evens /* : int */ = evens.accumulate((acc, x) -> acc + x)
    // one-liner:
    let sum_of_evens_2 = [1, 2, 3, 4, 5]
        .filter(x -> x % 2 == 0)
        .accumulate((acc, x) -> acc + x)
end
