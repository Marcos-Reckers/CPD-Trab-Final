import PySimpleGUI as sg

max_price = 500
block_column_size = 15
block_line_size = 10


layout = [
    [
        sg.Text('STEAM BUG'),
    ],
    [
        sg.Text('GENERO', justification= "center", size= block_column_size),sg.Text('IDIOMA', justification= "center", size= block_column_size),sg.Text('TAGS', justification= "center", size= block_column_size),sg.Text('DATA', justification= "center", size= block_column_size), sg.Text('DEV', justification= "center", size= block_column_size),sg.Text('PUBLISHER', justification= "center", size= block_column_size)
    ],
    [
        sg.Listbox(["a","b","c","d"], size=(block_column_size, block_line_size), key='-GENERO-'),
        sg.Listbox(["a","b","c","d"], size=(block_column_size, block_line_size), key='-IDIOMA-'),
        sg.Listbox(["a","b","c","d"], size=(block_column_size, block_line_size), key='-TAGS-'),
        sg.Listbox(list(range(1960, 2022)), size=(block_column_size, block_line_size), key='-DATA-'),
        sg.Listbox(["a","b","c","d"], size=(block_column_size, block_line_size), key='-DEV-'),
        sg.Listbox(["a","b","c","d"], size=(block_column_size, block_line_size), key='-PUBLISHER-')
    ],
    [
        sg.Push(),sg.Text('Preço MIN:', justification= "center"),sg.Push(), sg.Text('Preço MAX:', justification= "center")
    ],
    [
        sg.Slider(orientation="h",range=(0,max_price),resolution=0.01,key='-PRICE_MIN-',size=(60, 10)), sg.Slider(orientation="h",range=(0,max_price),resolution=0.01,default_value=max_price,key='-PRICE_MAX-')
    ],
    [
        [sg.Text('Review:')],
        [
            sg.Checkbox('Extremamente Positivas', size=(block_column_size * 2, 1)),sg.Checkbox('Muito Positivas', size=(block_column_size * 2, 1)),sg.Checkbox('Positivas', size=(block_column_size * 2, 1))
        ],
        [
            sg.Checkbox('Ligeiramente Positivas' , size=(block_column_size * 2, 1)),sg.Checkbox('Neutras', size=(block_column_size * 2, 1)),sg.Checkbox('Ligeiramente Negativas', size=(block_column_size * 2, 1))
        ],
        [
            sg.Checkbox('Negativas', size=(block_column_size * 2, 1)),sg.Checkbox('Muito Negativas', size=(block_column_size * 2, 1)),sg.Checkbox('Extremamente Negativas', size=(block_column_size * 2, 1))
        ]
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
