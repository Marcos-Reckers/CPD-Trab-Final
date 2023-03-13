import PySimpleGUI as sg
from PySimpleGUI import Column, VSeparator

max_price = 500
block_column_size = 15
block_line_size = 10

cabecalho = [
    [
        sg.Text('STEAM BUG'),
    ]
]

layout_listas = [

    [
        sg.Text('GENERO', justification="center", size=block_column_size), sg.Text('IDIOMA', justification="center", size=block_column_size), sg.Text('TAGS', justification="center", size=block_column_size), sg.Text('DATA', justification="center", size=block_column_size), sg.Text('DEV', justification="center", size=block_column_size), sg.Text('PUBLISHER', justification="center", size=block_column_size)
    ],
    [
        sg.Listbox(["a", "b", "c", "d"], size=(block_column_size, block_line_size), key='-GENERO-'),
        sg.Listbox(["a", "b", "c", "d"], size=(block_column_size, block_line_size), key='-IDIOMA-'),
        sg.Listbox(["a", "b", "c", "d"], size=(block_column_size, block_line_size), key='-TAGS-'),
        sg.Listbox(list(range(1960, 2022)), size=(block_column_size, block_line_size), key='-DATA-'),
        sg.Listbox(["a", "b", "c", "d"], size=(block_column_size, block_line_size), key='-DEV-'),
        sg.Listbox(["a", "b", "c", "d"], size=(block_column_size, block_line_size), key='-PUBLISHER-')
    ]
]

layout_price = [
    [
        sg.Push(), sg.Text('Preço MIN:', justification="center"), sg.Push(), sg.Text('Preço MAX:', justification="center"), sg.Push()
    ],
    [
        sg.Slider(orientation="h", range=(0, max_price), resolution=0.01, key='-PRICE_MIN-'), sg.Slider(orientation="h", range=(0, max_price), resolution=0.01, default_value=max_price, key='-PRICE_MAX-')
    ],
]

layout_reviews = [
    [
        [sg.Text('Review:')],
        [
            sg.Checkbox('Extremamente Positivas'), sg.Checkbox('Muito Positivas'), sg.Checkbox('Positivas')
        ],
        [
            sg.Checkbox('Ligeiramente Positivas'), sg.Checkbox('Neutras'), sg.Checkbox('Ligeiramente Negativas')
        ],
        [
            sg.Checkbox('Negativas'), sg.Checkbox('Muito Negativas'), sg.Checkbox('Extremamente Negativas')
        ]
    ],
    [
        sg.Text('AppID:'), sg.InputText(key='-ID-', size=10), sg.Text('Nome:'), sg.InputText(key='-NAME-')
    ],
    [sg.Button('BUSCAR'), sg.Button('Limpar Filtros')]
]

layout = [
    Column(layout_listas),
    VSeparator(),
    Column(layout_price),
    VSeparator(),
    Column(layout_reviews)
]

window = sg.Window(
    'Steam BUG',
    layout,
    element_justification='center'
)

while True:
    event, values = window.read()
    if event == sg.WIN_CLOSED:
        break
    elif event == 'BUSCAR':
        selected_price = values['-LISTBOX-'][0]
        print(selected_price)
window.close()
