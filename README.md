# Roguelike Shooter

- **Жанр**: Рогалик (Rogue-like), Шутер от первого лица (FPS)
- **Платформа**: ПК (Windows)
- **Игровой движок**: Unreal Engine 5 (C++/Blueprint)

## Описание

Roguelike Shooter - курсовой проект, сетевой шутер от первого лица с процедурной генерацией комнат.

## Геймплей и механики

Игроки появляются в центре случайной стартовой комнаты со случайным оружием. 

Все параметры оружия задаются в DataAsset - урон, скорость стрельбы, разброс, тип стрельбы (hit-scan или projectile) и тд. Также некоторые параметры рандомизируются в заданных пределах.

![Firearm Data Asset](https://github.com/CubeRuben/CubeRuben/blob/ab92ad41beeabd2e42ec8e132eb4f887e9f4050d/RoguelikeShooter/firearm-def.png?raw=true) 

Игроки могут подбирать и выкидывать оружия, также и переключаться между ними. Также у игроков есть боезапас каждого типа патронов, которые можно пополнять пикапами с пола и которыми необходимо перезаряжать оружие. 

![Firearm Pickups](https://github.com/CubeRuben/CubeRuben/blob/2093f5646a1251cf1b4a8fe1ac2d073dc11cc1d1/RoguelikeShooter/pickups.png?raw=true) 

Передвижение использует State Machine для обработки логики, и заимствует расчет векторов скорости из движка Source. Взрывы (бочки, гранаты и ракетницы) отталкивают игрока, из-за чего появляется механика Rocket jump (в игре есть отдельная ракетница с нулевым уроном для прыжков).

![Rocket Jump](https://github.com/CubeRuben/CubeRuben/blob/ab92ad41beeabd2e42ec8e132eb4f887e9f4050d/RoguelikeShooter/rocket-jump.png?raw=true) 

По уровню расставлены стационарные противники - турели. Есть две вариации: пулеметная и ракетная. С них могут выпасть новые оружия и расходники (патроны и аптечки).

![Turrets](https://github.com/CubeRuben/CubeRuben/blob/2093f5646a1251cf1b4a8fe1ac2d073dc11cc1d1/RoguelikeShooter/turrets.png?raw=true) 

Все действия игроков и параметры оружия реплицируются и синхронизированы между клиентами. 

## Демонстрация

![](https://github.com/CubeRuben/CubeRuben/blob/ab92ad41beeabd2e42ec8e132eb4f887e9f4050d/RoguelikeShooter/shot-01.png?raw=true) 
![](https://github.com/CubeRuben/CubeRuben/blob/ab92ad41beeabd2e42ec8e132eb4f887e9f4050d/RoguelikeShooter/shot-02.png?raw=true) 
![](https://github.com/CubeRuben/CubeRuben/blob/ab92ad41beeabd2e42ec8e132eb4f887e9f4050d/RoguelikeShooter/shot-03.png?raw=true) 
![](https://github.com/CubeRuben/CubeRuben/blob/ab92ad41beeabd2e42ec8e132eb4f887e9f4050d/RoguelikeShooter/shot-04.png?raw=true) 
