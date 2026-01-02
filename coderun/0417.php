<?php

// https://coderun.yandex.ru/problem/ambitious-snail
// greedy

final class FastScanner
{
    private string $buf;
    private int $pos = 0;
    private int $len;

    public function __construct()
    {
        $this->buf = file_get_contents("php://stdin");
        $this->len = strlen($this->buf);
    }

    public function nextInt(): int
    {
        $buf = $this->buf;
        $len = $this->len;
        $p = $this->pos;

        while ($p < $len) {
            if (ord($buf[$p]) > 32) {
                break;
            }
            $p++;
        }

        $sgn = 1;
        if ($p < $len && $buf[$p] === "-") {
            $sgn = -1;
            $p++;
        }

        $x = 0;
        while ($p < $len) {
            $c = ord($buf[$p]);
            if ($c < 48 || $c > 57) {
                break;
            }
            $x = $x * 10 + ($c - 48);
            $p++;
        }

        $this->pos = $p;
        return $x * $sgn;
    }
}

$fs = new FastScanner();
$n = $fs->nextInt();

$idx_pos = []; // up > dn
$idx_neg = []; // up <= dn

$base = 0;

$best_add = -1;
$best_id = 1;

for ($id = 1; $id <= $n; $id++) {
    $up = $fs->nextInt();
    $dn = $fs->nextInt();

    if ($up > $dn) {
        $base += $up - $dn;
        $idx_pos[] = $id;
        $add = $dn;
    } else {
        $idx_neg[] = $id;
        $add = $up;
    }

    if ($add > $best_add) {
        $best_add = $add;
        $best_id = $id;
    }
}

$ans = $base + $best_add;
fwrite(STDOUT, (string) $ans . "\n");

$out = "";
$first = true;
$lim = 1 << 20;

$emit = function (int $v) use (&$out, &$first, $lim) {
    if ($first) {
        $out .= (string) $v;
        $first = false;
    } else {
        $out .= " " . $v;
    }
    if (strlen($out) >= $lim) {
        fwrite(STDOUT, $out);
        $out = "";
    }
};

foreach ($idx_pos as $v) {
    if ($v === $best_id) {
        continue;
    }
    $emit($v);
}
$emit($best_id);
foreach ($idx_neg as $v) {
    if ($v === $best_id) {
        continue;
    }
    $emit($v);
}

fwrite(STDOUT, $out . "\n");
