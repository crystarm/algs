<?php

// https://coderun.yandex.ru/problem/hack-pentagon

$line = fgets(STDIN);
if ($line === false) exit;
$n = (int)trim($line);

$baseString = str_repeat('a', $n);

echo $baseString . "\n";
flush();

$response = fgets(STDIN);
if ($response === false) exit;
$baseDist = (int)trim($response);

if ($baseDist === 0) {
    exit;
}

$password = '';

for ($i = 0; $i < $n; $i++) {
    $baseString[$i] = 'z';

    echo $baseString . "\n";
    flush();

    $response = fgets(STDIN);
    if ($response === false) exit;
    $currentDist = (int)trim($response);

    if ($currentDist === 0) {
        exit;
    }

    $baseString[$i] = 'a';

    $delta = $currentDist - $baseDist;
    $offset = (25 - $delta) / 2;

    $char = chr(ord('a') + (int)$offset);
    $password .= $char;
}

echo $password . "\n";
flush();

fgets(STDIN);
