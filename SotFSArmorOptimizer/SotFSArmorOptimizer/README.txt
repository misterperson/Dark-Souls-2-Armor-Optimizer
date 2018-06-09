This is a command line utility to give the best combination of armors
given a build's stat and weight requirements.
Edit "Input.json" to configure the program for your build and needs

"Max Burden": how much your character can wear at 100% burden
"Weight Without Armor": How much your weapons and rings weigh
"Target Equipload": a number from 0 to 1 representing the percentage burden
                    you would like your build to be

Stats:
"Stat": stat
Enter your build's stats without any armor equiped

Constraints:
"Stat": [ minimum, maximum ]
Constraints will only select armor sets
 that have stats within the range provided
 

 Optimize For: The stat to optimize for when all constraints are otherwise met
 0 = poise
 1 = physical, 2 = strike, 3 = slash, 4 = thrust
 5 = magic, 6 = fire, 7 = lightning, 8 = dark,
 9 = poison, 10 = bleed, 11 = petrification, 12 = curse