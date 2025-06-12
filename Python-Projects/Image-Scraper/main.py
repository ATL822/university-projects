# CSCI-3038 - Project 5
# Andrew Law
# Dr.Smith
# August 02, 2024

# NOTE: this project was tested on Dr.Untch's, Dr.Dong's, and Dr.Phillips's MTSU webpages.

import urllib.request
import tkinter as tk

# global GUI vars
root = tk.Tk()
root.title('Andrew\'s SUPER COOL Project 5')
root.configure(background='darkgrey')
root.minsize(500,300)
root.maxsize(500,300)
root.geometry('500x300+500+300') # set start position of the window

link = tk.StringVar() # global variable to hold link from entry box
image_link_tails = [] # store image links, used as value in tk variable below
var = tk.Variable(value=image_link_tails)


def main():
    #GUI components
    prompt = tk.Label(root, text='Enter a link below and get a\nlist of all images on that page!', font=('Helvetica', 18), justify='center', bg='lightgrey', fg='Navy')
    prompt.pack()

    link_field = tk.Entry(root, font=('Helvetica', 18), textvariable=link)
    link_field.pack()

    get_link_button = tk.Button(root, text='Enter', font=('Helvetica', 18), command=display_images, bg='lightgrey', fg='Navy', activebackground='Navy', activeforeground='lightgrey')
    get_link_button.pack()

    photo_list_box = tk.Listbox(root, listvariable=var)
    photo_list_box.pack(expand=True, fill=tk.BOTH)

    download_button = tk.Button(root, text='Download Images', font=('Helvetica', 18), command=download_images, bg='lightgrey', fg='Navy', activebackground='Navy', activeforeground='lightgrey')
    download_button.pack()

    root.mainloop()


# get the images from the webpage and store them in the tk variable 'var'
def display_images():
    page_link = link.get()
    print(f'attempting to open: {page_link}')
    
    try:
        image_link_tails = [] # reset link tail list
        with urllib.request.urlopen(page_link) as response:
            page = response.read()
            for line in page.decode('utf-8').split('\n'):
                # get only images with .jpg or .png
                if (line.__contains__('.jpg') or line.__contains__('.png')):
                    split_line = line.split('"')
                    for part in split_line:
                        if part.__contains__('.jpg') or part.__contains__('.png'):
                            image_link_tails.append(part)
        var.set(value=image_link_tails)
    except Exception as e:
        print(f'Error with entry: {e}')


# download the images from the webpage
def download_images():
    page_link = link.get()
    count = 0
    print(f'downloading images from {link.get()}...')
    for link_tail in var.get():
        # had to do some splitting and joining to the link tails so the proper image link is used in the urlretrieve later.
        link_tail_parts = link_tail.split('/')
        for part in link_tail_parts:
            # '~' is removed from the link_tail so there is not a duplicate in the image_link, the ~ is consistant across the professors' webpages. This is not a universal pattern across the internet but for testing on different professor's pages it works.
            if part.__contains__('~'): 
                link_tail_parts.remove(part)
        link_tail = '/'.join(link_tail_parts)

        # set the correct file extension
        if link_tail.__contains__('.jpg'):
            image_type = '.jpg'
        else:
            image_type = '.png'
        image_name = 'image' + str(count) + image_type
        image_link = page_link + link_tail
        print('getting image from link: ' + image_link)
        try:
            urllib.request.urlretrieve(image_link, image_name)
        except Exception as e:
            print(f'Could not download images. Error: {e}')
        count += 1
    


if __name__ == '__main__':
    main()
