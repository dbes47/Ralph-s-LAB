number = 100
status = True


def number_identifier(begin, end):
    for i in range(begin, end):
        if i % 2 == 0:
            print "even Number", i
        elif 1 % 2 == 1:
            print "odd Number", i


while status == True:
    input_valu = int(raw_input())
    if input_valu != number:
        status = False
        number_identifier(1, input_valu)
        print 'Matched'
    else:
        print ''' Not matched '''