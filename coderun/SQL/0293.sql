-- https://coderun.yandex.ru/problem/sql-users-active-sessions

WITH t AS (
  SELECT
    user_id,
    time,
    LAG(time) OVER (PARTITION BY user_id ORDER BY time) AS prev_time
  FROM logs
)
SELECT
  user_id,
  SUM(CASE
        WHEN prev_time IS NULL OR time - prev_time > 30 THEN 1
        ELSE 0
      END) AS cnt
FROM t
GROUP BY user_id
ORDER BY user_id;
