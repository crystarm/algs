// https://coderun.yandex.ru/problem/calculator

<?php

$n = 0;
fscanf(STDIN, "%d", $n);

$dp = [];
$prev = [];

$dp[1] = 0;
$prev[1] = 0;

for ($i = 2; $i <= $n; $i++) {
    $dp[$i] = $dp[$i - 1] + 1;
    $prev[$i] = $i - 1;

    if ($i % 2 == 0) {
        if ($dp[$i / 2] + 1 < $dp[$i]) {
            $dp[$i] = $dp[$i / 2] + 1;
            $prev[$i] = $i / 2;
        }
    }

    if ($i % 3 == 0) {
        if ($dp[$i / 3] + 1 < $dp[$i]) {
            $dp[$i] = $dp[$i / 3] + 1;
            $prev[$i] = $i / 3;
        }
    }
}

echo $dp[$n] . "\n";

$path = [];
$curr = $n;
while ($curr >= 1) {
    $path[] = $curr;
    if ($curr == 1) break;
    $curr = $prev[$curr];
}

echo implode(" ", array_reverse($path));

?>
