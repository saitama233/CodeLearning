from PIL import Image
from PIL.ExifTags import TAGS
import sys

def get_exit_data(fname):
    """Get embeded EXIT data from image file."""
    ret = {}
    try:
        img = Image.open(fname)
        if hasattr(img, '_getexit'):
            exitinfo = img._getexit()
            if exitinfo != None:
                for tag, value in exitinfo.items():
                    decoded = TAGS.get(tag, tag)
                    ret[decoded] = value
    except IOError:
        print 'IOError ' + fname
    return ret

def main():
    for filename in sys.argv[1:]:
        ret = get_exit_data(filename)
        print ret

if __name__ == '__main__':
    main()
