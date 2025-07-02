# Задание 3

**Дано**: набор данных в таблице players со структурой полей:
 `players(id INT, name VARCHAR, login_time DATETIME, device INT)`

**Задание**: сформулировать SQL-запросы для следующих случаев:

___

Получить **5 самых активных устройств** – то есть пять значений поля `device`, с которых произошло наибольшее количество логинов пользователей (упорядочить по количеству логинов).

```sql
SELECT device, COUNT(*) AS login_count
FROM players
GROUP BY device
ORDER BY login_count DESC
LIMIT 5;
```

___

Получить **среднее число логинов в день** за последние 7 дней (в расчете от текущей даты).

```sql
SELECT 
    COUNT(*) / 7.0 AS avg_logins_per_day
FROM players
WHERE login_time >= CURDATE() - INTERVAL 7 DAY;
```

___
