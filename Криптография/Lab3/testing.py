import random
import string
import logging
import hashlib
import bitstring
import sha256
from sha256 import *
from hashlib import sha256

def genRandStr(size):
    k = ""
    for i in range(size):
        k += random.choice('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ')
    return k


def changeLast(string):
    lastS = string[- 1]
    prevPart = string[:- 1]
    randomList = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
    ind = randomList.index(lastS)
    newList = randomList[:ind] + randomList[(ind+1):]
    return prevPart + random.choice(newList)


def origSha256(string):
    return sha256(string.encode('utf-8')).hexdigest()


def checkingOK(tests):
    okTests = 0
    for i in range(tests):
        theString = genRandStr(random.randrange(3, 50))
        ans1 = sha_256(theString)
        ans2 = origSha256(theString)
        if ans1 == ans2 : okTests += 1
    return "OK tests: " + str(okTests) + "/" + str(tests)



def stringDiff(str1, str2):
    diff = 0
    minL = min(len(str1), len(str2))
    maxL = max(len(str1), len(str2))
    diff += maxL - minL
    for i in range(minL):
        if str1[i] != str2[i]:
            diff += 1
    return diff



def cryptoAnalysis(start, end, mults = 0):
    for k in range(start, end+1):
        score = 0
        #3 строки из 2 символов
        for i in range(3):
            newS = genRandStr(2)
            newS2 = changeLast(newS)
            sha1 = sha_256(newS, k)
            sha2 = sha_256(newS2, k)
            score += stringDiff(sha1, sha2) * (1 + (mults*1))

        #4 строки из 5 символов
        for i in range(4):
            newS = genRandStr(5)
            newS2 = changeLast(newS)
            sha1 = sha_256(newS, k)
            sha2 = sha_256(newS2, k)
            score += stringDiff(sha1, sha2) * (1 + (mults*4))

        #5 строк из 10 символов
        for i in range(5):
            newS = genRandStr(10)
            newS2 = changeLast(newS)
            sha1 = sha_256(newS, k)
            sha2 = sha_256(newS2, k)
            score += stringDiff(sha1, sha2) * (1 + (mults*9))

        #6 строк из 20 символов
        for i in range(6):
            newS = genRandStr(20)
            newS2 = changeLast(newS)
            sha1 = sha_256(newS, k)
            sha2 = sha_256(newS2, k)
            score += stringDiff(sha1, sha2) * (1 + (mults*19))

        #7 строк из 50 символов
        for i in range(7):
            newS = genRandStr(50)
            newS2 = changeLast(newS)
            sha1 = sha_256(newS, k)
            sha2 = sha_256(newS2, k)
            score += stringDiff(sha1, sha2) * (1 + (mults*49))

        #10 строк из 100 символов
        for i in range(10):
            newS = genRandStr(100)
            newS2 = changeLast(newS)
            sha1 = sha_256(newS, k)
            sha2 = sha_256(newS2, k)
            score += stringDiff(sha1, sha2) * (1 + (mults*99))

        #15 строк из 500 символов
        for i in range(15):
            newS = genRandStr(500)
            newS2 = changeLast(newS)
            sha1 = sha_256(newS, k)
            sha2 = sha_256(newS2, k)
            score += stringDiff(sha1, sha2) * (1 + (mults*499))

        
        print("Rounds: " + str(k) + " = " + str(score))
    
        


