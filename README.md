# sunroofauto
arduino esp32 projects
Hi,
I have been trying for some time to automate a simple sunroof.
It works fine so far.


My plans for it are a bit ambitious for me:

I want a (semi) automated sunroof in my serre that closes when it starts to rain, with cold weather and opens with sun and hot. I may want to open it manually too, (for letting bbq fumes out). I may want to check the weather at home when I'm not there and open/close the greenhouse windows. I think I want a new greenhouse next year. It should be watered automatically depending different plants and weather.

What I have now:
Sunroof operated by 12v to a flatbed-scanner motor and gear. Motor direction and distance operated with 4 relay by Arduino-uno with 4 buttons. Limits are set by simple switches on interrupt functions. There are some interrupt functions programmed but never implemented to enable an external Arduino to open/close the roof by connecting 2 pins to ground.
This existing situation could be rebuilt to an esp32 because the Arduino eats power and I switch it off when not in use.

The new station and attachments should be powered by my existing 12v solar system. The solar system powers also the led lights and hanging fans, so power is limited. (money is limited too, time I have enough but due to limited health I can only do so much a day)
I am thinking the weatherstation can be online 24/7 but it does not need to open/close windows and roof every minute. maybe check weather every half hour or so.
secondary projects: I would like to have an automated reminder when my washing machine has finished , because I forget it all the time.

Any suggestions and tips are welcome. I'm enrolled in the esp32 course and find the environment very suitable for my projects.

