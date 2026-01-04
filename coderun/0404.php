<?php

// longest increasing subsequence + greedy approach fail

ini_set("memory_limit", "256M");

function readInput()
{
    $input = "";
    while ($line = fgets(STDIN)) {
        $input .= $line . " ";
    }
    return preg_split("/\s+/", trim($input));
}

$tokens = readInput();

if (count($tokens) < 1) {
    exit(0);
}

$n = (int) $tokens[0];
$books = [];

for ($i = 0; $i < $n; $i++) {
    if (isset($tokens[$i + 1])) {
        $books[] = (int) $tokens[$i + 1];
    }
}

$memo = [];

function solve($idx, $left, $right)
{
    global $books, $n, $memo;

    if ($idx == $n) {
        return 0;
    }

    if (isset($memo[$idx][$left][$right])) {
        return $memo[$idx][$left][$right];
    }

    $currentH = $books[$idx];
    $res = solve($idx + 1, $left, $right);

    if ($currentH >= $left && $currentH <= $right) {
        $takeLeft = 1 + solve($idx + 1, $currentH, $right);
        $takeRight = 1 + solve($idx + 1, $left, $currentH);

        if ($takeLeft > $res) {
            $res = $takeLeft;
        }
        if ($takeRight > $res) {
            $res = $takeRight;
        }
    }

    $memo[$idx][$left][$right] = $res;
    return $res;
}

echo solve(0, 0, 101);
