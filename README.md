# Chained Commands

This small Qt application can execute multiple commands one after another.

This is useful if you want to stack commands and add/edit/remove them on the fly.

Contrary to a script (.bat, .ps1, .sh), you can add/edit/remove a command before it is executed.
It is also possible to skip a command (terminate/kill the process) and to pause after the execution of the current command.

This project uses Qt 6.4.2 MinGW

## Examples

### Stack commands

If you need to run 10 commands quickly, you can execute them in parallel with 10 terminals.
But if each command uses 100 % of your resources, it's better to execute one by one.

You could create a script, but it means you have to write your 10 commands before starting the operation.
And if you need to modify a command, you have to stop the script...

With this application, you can write/paste the first commands, execute them and, while they are running, write the last commands.
It is also possible to execute commands before they are executed.

### 2 commands using the same port

A process is running on port 1234.
You want to stop this process and start another one on the same port 1234.
In a terminal, you would stop the command and start the other with 2–10 seconds without connection.
With this application, you can add the new command and run it 0.5 seconds after the previous one ended.
