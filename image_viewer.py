from PIL import Image 
import sys 

def run():
    n = len(sys.argv)

    im = Image.open(sys.argv[1], "r")

    im.show()

if __name__ == "__main__":
    run()