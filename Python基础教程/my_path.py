# coding=utf-8
def square(x):
    '''
    Squares a number and returns the result.
    >>> square(3)
    9
    >>> square(5)
    25
    '''
#    return x*x+1
    return x*x

if __name__=='__main__':
    import doctest,my_path
    doctest.testmod(my_path)
