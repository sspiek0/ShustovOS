#!/usr/bin/python3

import subprocess

textBold = '\033[1m'
textColorRed = '\033[31m'
textColorGreen = '\033[32m'
textReset = '\033[0m'

width = 50
greetMsg = 'Welcome To ShustovOS!'

users = {'sabubu': 'lex', 'urMom26': '1185', 'yappil': '1185'}



print('*'*width)
print(' '*((width-len(greetMsg))//2)+f'{textBold}{greetMsg}{textReset}'+' '*((width-len(greetMsg))//2))
print('*'*width+"\n\n\n")

print(f'{textBold}  -Log in system-{textReset}', '\n')

username = input('-- Username: ')

while True:
    if username in users:
        password = users[username]
        break
    else:
        print(f'\n{textColorRed}! No such user. Try again.{textReset}\n')
        username = input('-- Username: ')

print()

while True:
    enteredPassword = input('-- Password: ')
    if enteredPassword == password:
        break
    else:
        print(f'\n{textColorRed}! Wrong password. Try again.{textReset}\n')

print(f"\n\n{textColorGreen}{textBold}✓ Welcome, {username}!{textReset}")

subprocess.run(["/bin/term"])