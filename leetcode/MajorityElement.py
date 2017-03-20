


class Solution:
    # @param num, a list of integers
    # @return an integer
    def majorityElement1(self, num):
        d = {}
        for i in num:
            if i not in d:
                d[i] = 1
            else:
                d[i] += 1
        n = 0
        c = 0
        for k in d:
            v = d[k]
            if v > c:
                n = k
                c = v
        return n

    def majorityElement(self, num):
        c = 0
        n = 0
        for i in num:
            if c == 0:
                n = i
                c = 1
                continue
            if n == i:
                c += 1
            else:
                c -= 1
        return n


if __name__ == "__main__":
    l = [1,2,3,1,1,1]
    from timeit import Timer
    t1 = Timer("Solution().majorityElement1(l)", "from __main__ import Solution, l")
    t2 = Timer("Solution().majorityElement(l)", "from __main__ import Solution, l")

    print t1.repeat(3, 1000)
    print t2.repeat(3, 1000)

    print Solution().majorityElement(l)


