from PIL import Image, ImageFont, ImageDraw
from textwrap import wrap


class Steganography:

    @staticmethod
    def __write_text(input_text, img_size):
        img_text = Image.new("RGB", img_size)

        drawer = ImageDraw.Draw(img_text)
        offset = 50
        for line in wrap(input_text, width=30):
            drawer.text((40, offset), line, font=ImageFont.load_default().font)
            offset += 10
        return img_text

    @staticmethod
    def encode(input_text, input_image, enc_img_path):
        rc, gc, bc, = input_image.split()

        image_text = Steganography.__write_text(input_text, input_image.size)
        bw_encode = image_text.convert('1')

        enc_img = Image.new('RGB', (input_image.size[0], input_image.size[1]))
        pixels = enc_img.load()
        for i in range(input_image.size[0]):
            for j in range(input_image.size[1]):
                red_template_pix = bin(rc.getpixel((i, j)))
                if bin(bw_encode.getpixel((i, j)))[-1] == '1':
                    red_template_pix = red_template_pix[:-1] + '1'
                else:
                    red_template_pix = red_template_pix[:-1] + '0'
                pixels[i, j] = (int(red_template_pix, 2), gc.getpixel((i, j)), bc.getpixel((i, j)))

        enc_img.save(enc_img_path)

    @staticmethod
    def decode(encode_img, img_path):
        rc = encode_img.split()[0]

        decode_img = Image.new('RGB', encode_img.size)
        pixels = decode_img.load()
        for i in range(encode_img.size[0]):
            for j in range(encode_img.size[1]):
                if bin(rc.getpixel((i, j)))[-1] == '0':
                    pixels[i, j] = (255, 255, 255)
                else:
                    pixels[i, j] = (0, 0, 0)
        decode_img.save(img_path)


if __name__ == '__main__':
    text = """To be, or not to be--that is the question:
Whether 'tis nobler in the mind to suffer
The slings and arrows of outrageous fortune
Or to take arms against a sea of troubles
And by opposing end them. To die, to sleep--
No more--and by a sleep to say we end
The heartache, and the thousand natural shocks
That flesh is heir to. 'Tis a consummation
Devoutly to be wished. To die, to sleep--
To sleep--perchance to dream: ay, there's the rub,
For in that sleep of death what dreams may come
When we have shuffled off this mortal coil,
Must give us pause. There's the respect
That makes calamity of so long life."""
    encode_img_path = 'encode.png'
    input_img = Image.open('google.jpg')
    Steganography.encode(text, input_img, encode_img_path)
    encode_image = Image.open(encode_img_path)
    Steganography.decode(encode_image, 'decode.png')