# Crisps Arith
　This is a multifuntional app of **mental arithmetic** for Chinese **elementary school students**, whose graphics library is [EasyX](https://docs.easyx.cn/).  
　Its language is **Chinese**. There's **no** schedule to make an **English or other languages** version for it.
## Programme
　Though its algorithm isn't perfect enough, if you want to copy this code to reprogramme it, **please install EasyX in your PC at first**.  
　And using VS2022, create the project as **Windows desktop appliction**.
## Instuction
　There are four **scenes**, ***Main Menu*, *Login*, *Arithmetic*, and *Admin***. 
It **starts from *Login*** to input your ID. You can switch to ***Admin*** by inputing **admin**. 
If what's inputed is a legal ID except **admin**, it'll switch to ***Arithmetic***. 
As long as the **esc** is pressed any enough times at any scene, there will be a pop-up message to switch to ***Main Menu***. 
When in ***Main Menu***, press **esc** to quit the app or input one legal **scene name** to switch to other scenes, which are ***kousuan* for *Login*, *kousuan admin* for *Admin*, *caidan* for *Main Menu*, and *tuichu* to quit this app**. 

　When stated for the first time, the app will automatically create a folder to contain users' data. 
When generating the first user, it will create a file ***password.bin***, the initial password of which is **password**. 
When you try to use the ***Admin***, you should input the password after inputing the target user's ID, though the password is global for encrypting the users' data. 
And the target varible names are limited, please check the **specific names** and **input sequence** in the code. 
For specific names, get them in [pre.h *const vector<string> legal_varible*](./pre.h#L24); for input sequence, get it in [next.h *void Admin::proceed(clock_t delta)*](./next.h#L702).
