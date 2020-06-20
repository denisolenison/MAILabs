import random
import string


def getFile(filename):
    f = open(filename, 'r')
    content = f.read()
    return content
    f.close()

def makeRandomWord():
    f = open('dict.txt', 'r')
    words = f.read()
    words = words.splitlines()
    f.close()
    return random.choice(words)

def randomWordText(amount):
    f = open('dict.txt', 'r')
    words = f.read()
    words = words.splitlines()
    f.close()
    answer = ''
    answer += random.choice(words) + ' '
    for i in range (amount-1):
        answer += random.choice(words)
        if i < amount-2:
            answer += random.choice([' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ',
                                     ' ',' ',' ', ' ', ' ', ' ', ' ',' ', ' ',' ',
                                     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                     ' ', ' ', ' ', ' ', '\n', '\n', '\n', '\n\n'])
    return answer


def writeToFile(filename, text):
    f = open(filename, "a")
    f.write(text)
    f.close()
    


def makeRandomStupidWord():
    word = ''
    word += random.choice('АБВГДЕЁЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзиклмнопрстуфхцчшщъыьэюя')
    for i in range (random.randrange(11)):
        word += random.choice('АБВГДЕЁЖЗИКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯабвгдеёжзиклмнопрстуфхцчшщъыьэюя')
    return word

def randomStupidWordText(amount):
    answer = ''
    answer += makeRandomStupidWord() + ' '
    for i in range (amount-1):
        answer += makeRandomStupidWord()
        if i < amount-2:
            answer += random.choice([' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',' ',
                                     ' ',' ',' ', ' ', ' ', ' ', ' ',' ', ' ',' ',
                                     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                     ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
                                     ' ', ' ', ' ', ' ', '\n', '\n', '\n', '\n\n'])
    return answer

def randomStupidWordTextNoSpace(amount):
    answer = ''
    for i in range (amount):
        answer += makeRandomStupidWord()
    return answer


def compareText(text1, text2):
    score = 0
    commonL = min(len(text1), len(text2))
    for i in range(commonL):
        if text1[i] == text2[i]:
            score += 1
    return commonL, score


def mainTests(text1, text2, text3, text4, text5, text6, text7, text8):
    f1 = open(text1, 'r')
    f2 = open(text2, 'r')
    f3 = open(text3, 'r')
    f4 = open(text4, 'r')
    f5 = open(text5, 'r')
    f6 = open(text6, 'r')
    f7 = open(text7, 'r')
    f8 = open(text8, 'r')
    txt1 = f1.read()
    txt2 = f2.read()
    txt3 = f3.read()
    txt4 = f4.read()
    txt5 = f5.read()
    txt6 = f6.read()
    txt7 = f7.read()
    txt8 = f8.read()
    f1.close()
    f2.close()
    f3.close()
    f4.close()
    f5.close()
    f6.close()
    f7.close()
    f8.close()
    
    print('Случай 1: 2 осмысленных текста на русском языке\n')
    len1, com1 = compareText(txt1, txt2)
    print('Общая длина текстов = ' + str(len1))
    print('Совпало символов = ' + str(com1))
    print('Процент = ' + str(round(com1/len1*100, 5)) + '%\n')
    
    print('Случай 2: Осмысленный текст и текст из рандомных букв\n')
    len1, com1 = compareText(txt1, txt5)
    len2, com2 = compareText(txt1, txt6)
    len3, com3 = compareText(txt2, txt5)
    len4, com4 = compareText(txt2, txt6)
    perc1, perc2, perc3, perc4 = (com1/len1), (com2/len2), (com3/len3), (com4/len4)
    print('Тексты 1 и 1:')
    print('Общая длина текстов  = ' + str(len1))
    print('Совпало символов = ' + str(com1))
    print('Процент = ' + str(round(perc1*100, 5)) + '%\n')
    
    print('Тексты 1 и 2:')
    print('Общая длина текстов  = ' + str(len2))
    print('Совпало символов = ' + str(com2))
    print('Процент = ' + str(round(perc2*100, 5)) + '%\n')
    
    print('Тексты 2 и 1:')
    print('Общая длина текстов  = ' + str(len3))
    print('Совпало символов = ' + str(com3))
    print('Процент = ' + str(round(perc3*100, 5)) + '%\n')
    
    print('Тексты 2 и 2:')
    print('Общая длина текстов  = ' + str(len4))
    print('Совпало символов = ' + str(com4))
    print('Процент = ' + str(round(perc4*100, 5)) + '%\n')
    
    print('Средний процент = ' + str(round((perc1+perc2+perc3+perc4)*25, 5)) + '%\n')
    
    print('Случай 3: Осмысленный текст и текст из рандомных слов\n')
    len1, com1 = compareText(txt1, txt3)
    len2, com2 = compareText(txt1, txt4)
    len3, com3 = compareText(txt2, txt3)
    len4, com4 = compareText(txt2, txt4)
    perc1, perc2, perc3, perc4 = (com1/len1), (com2/len2), (com3/len3), (com4/len4)
    print('Тексты 1 и 1:')
    print('Общая длина текстов  = ' + str(len1))
    print('Совпало символов = ' + str(com1))
    print('Процент = ' + str(round(perc1*100, 5)) + '%\n')
    
    print('Тексты 1 и 2:')
    print('Общая длина текстов  = ' + str(len2))
    print('Совпало символов = ' + str(com2))
    print('Процент = ' + str(round(perc2*100, 5)) + '%\n')
    
    print('Тексты 2 и 1:')
    print('Общая длина текстов  = ' + str(len3))
    print('Совпало символов = ' + str(com3))
    print('Процент = ' + str(round(perc3*100, 5)) + '%\n')
    
    print('Тексты 2 и 2:')
    print('Общая длина текстов  = ' + str(len4))
    print('Совпало символов = ' + str(com4))
    print('Процент = ' + str(round(perc4*100, 5)) + '%\n')
    
    print('Средний процент = ' + str(round((perc1+perc2+perc3+perc4)*25, 5)) + '%\n')
    
    print('Случай 4: 2 текста из рандомных букв (с пробелами и пер. строк)\n')
    len1, com1 = compareText(txt5, txt6)
    print('Общая длина текстов = ' + str(len1))
    print('Совпало символов = ' + str(com1))
    print('Процент = ' + str(round(com1/len1*100, 5)) + '%\n')
    
    print('Случай 5: 2 текста из рандомных слов\n')
    len1, com1 = compareText(txt3, txt4)
    print('Общая длина текстов = ' + str(len1))
    print('Совпало символов = ' + str(com1))
    print('Процент = ' + str(round(com1/len1*100, 5)) + '%\n')
    
    print('Случай 6: Тексты из рандомных слов и рандомных букв\n')
    len1, com1 = compareText(txt3, txt5)
    len2, com2 = compareText(txt3, txt6)
    len3, com3 = compareText(txt4, txt5)
    len4, com4 = compareText(txt4, txt6)
    perc1, perc2, perc3, perc4 = (com1/len1), (com2/len2), (com3/len3), (com4/len4)
    print('Тексты 1 и 1:')
    print('Общая длина текстов  = ' + str(len1))
    print('Совпало символов = ' + str(com1))
    print('Процент = ' + str(round(perc1*100, 5)) + '%\n')
    
    print('Тексты 1 и 2:')
    print('Общая длина текстов  = ' + str(len2))
    print('Совпало символов = ' + str(com2))
    print('Процент = ' + str(round(perc2*100, 5)) + '%\n')
    
    print('Тексты 2 и 1:')
    print('Общая длина текстов  = ' + str(len3))
    print('Совпало символов = ' + str(com3))
    print('Процент = ' + str(round(perc3*100, 5)) + '%\n')
    
    print('Тексты 2 и 2:')
    print('Общая длина текстов  = ' + str(len4))
    print('Совпало символов = ' + str(com4))
    print('Процент = ' + str(round(perc4*100, 5)) + '%\n')
    
    print('Средний процент = ' + str(round((perc1+perc2+perc3+perc4)*25, 5)) + '%\n')

    print('Случай 7: 2 текста из рандомных букв (без пробелов и пер. строк)\n')
    len1, com1 = compareText(txt7, txt8)
    print('Общая длина текстов = ' + str(len1))
    print('Совпало символов = ' + str(com1))
    print('Процент = ' + str(round(com1/len1*100, 5)) + '%\n')



mainTests('StephenKing - Institute.txt', 'NealStephenson - Anathem.txt',
          'randWordText1.txt', 'randWordText2.txt', 'stupid1.txt',
          'stupid2.txt', 'stupid1_no_space.txt', 'stupid2_no_space.txt')
