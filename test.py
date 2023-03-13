import PySimpleGUI as sg

max_price = 500

layout = [
    [
        sg.Text('STEAM BUG'),
    ],
    [
        sg.Text('GENERO'),sg.Text('IDIOMA'),sg.Text('TAGS'),sg.Text('DATA'),
    ],
    [
        sg.Listbox(["a","b","c","d"], size=(15, 15), key='-GENERO-'),
        sg.Listbox(["a","b","c","d"], size=(15, 15), key='-IDIOMA-'),
        sg.Listbox(["a","b","c","d"], size=(15, 15), key='-TAGS-'),
        sg.Listbox(list(range(1960, 2022)), size=(15, 15), key='-DATA-')
    ],
    [
        sg.Text('DEV'),sg.Text('PUBLISHER')
    ],
    [
        sg.Listbox(["a","b","c","d"], size=(15, 15), key='-DEV-'),
        sg.Listbox(["a","b","c","d"], size=(15, 15), key='-PUBLISHER-')
    ],
    [
        sg.Text('Preço MIN:'), sg.Text('Preço MAX:')
    ],
    [
        sg.Slider(orientation="h",range=(0,max_price),resolution=0.01,key='-PRICE_MIN-'), sg.Slider(orientation="h",range=(0,max_price),resolution=0.01,default_value=max_price,key='-PRICE_MAX-')
    ],
    [
        sg.Text('Review:'),sg.Checkbox('Extremamente Positivas'),sg.Checkbox('Muito Positivas'),sg.Checkbox('Positivas'),sg.Checkbox('Ligeiramente Positivas'),sg.Checkbox('Neutras'),sg.Checkbox('Ligeiramente Negativas'),sg.Checkbox('Negativas'),sg.Checkbox('Muito Negativas'),sg.Checkbox('Extremamente Negativas')
    ],
    [
        sg.Text('AppID:'), sg.InputText(key='-ID-'), sg.Text('Nome:'), sg.InputText(key='-NAME-')
    ],
    [sg.Button('BUSCAR'), sg.Button('Limpar Filtros')]
]

window = sg.Window('Steam BUG', layout)

while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED:
        break
    elif event == 'BUSCAR':
        selected_price = values['-LISTBOX-'][0]
        print(selected_price)
window.close()
