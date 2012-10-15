import random
import string
import os, sys
import md5
import subprocess as sb

alphabet = [chr(i) for i in xrange(33, 256)]
print alphabet
def randstr(maxlen=10000):
    return ''.join(random.choice(alphabet) for i in xrange(maxlen))

def test(s):
    print 'Test:[%s]' % (s if len(s) < 20 else s[:20] + '...')
    ans1 = md5.md5(s).hexdigest()
    input = "/tmp/md5_input"
    open(input, 'w').write(s)
    p = sb.Popen(["./md5sum", input], stdout=sb.PIPE)
    p.wait()
    ans2 = p.stdout.read().replace(' ', '')
    if ans1 == ans2:
        print 'Pass'
        return True
    else:
        print 'Failed.'
        print '  excepted: %s\n  got:      %s \n' %(ans1, ans2)
        return False

def main():
    test("")
    test("a")
    n = 10
    p = 0
    for i in xrange(n):
        s = randstr(10000)
        if test(s):
            p += 1
    print 'Passed %d of %d' %(p, n)
main()
