-- https://coderun.yandex.ru/problem/genres
-- data traversal problem

WITH RECURSIVE track_full_genres AS (

    SELECT
        tg.track_id,
        g.id AS genre_id,
        g.name AS genre_name,
        g.parent_genre_id
    FROM
        track_genre tg
    JOIN
        genre g ON tg.genre_id = g.id

    UNION ALL

    SELECT
        tfg.track_id,
        g.id AS genre_id,
        g.name AS genre_name,
        g.parent_genre_id
    FROM
        track_full_genres tfg
    JOIN
        genre g ON tfg.parent_genre_id = g.id
)
SELECT DISTINCT
    tfg.track_id,
    tfg.genre_id,
    t.name AS track_name,
    tfg.genre_name
FROM
    track_full_genres tfg
JOIN
    track t ON tfg.track_id = t.id
ORDER BY
    tfg.track_id ASC,
    tfg.genre_id ASC;
