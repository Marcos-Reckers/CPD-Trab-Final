import PySimpleGUI as sg
import subprocess
import os


class windows:
    def __init__(self, DB_PATH: str):
        self.DB_PATH = DB_PATH
        if not os.path.isfile(self.DB_PATH):
            self.DB_fail_popup()
            return

        validate = subprocess.run([self.DB_PATH, '-v'], capture_output=True)
        if '0' in validate.stdout.decode('utf-8'):
            if not self.create_DB():
                return

        self.block_column_size = 18  # Tamanho das colunas
        self.block_line_size = 10  # Tamanho das linhas

        self.load_fields()
        self.create_menu()

        self.window_menu = sg.Window(
            'Steam BUG',
            self.menu,
            element_justification='center'
        )

        self.main_window()

    def DB_fail_popup(self):
        """Cria a janela dizendo que o programa de base de dados não existe"""
        sg.popup('O programa de base de dados não existe')

    def create_DB(self) -> bool:
        """Cria a janela para procurar e criar a base de dados"""
        create_DB_screen = [
            [sg.Text('Caminho para DB')],
            [sg.Combo(sg.user_settings_get_entry('-filenames-', []), default_value=sg.user_settings_get_entry(
                '-last filename-', ''), size=(80, 1), key='-FILENAME-'), sg.FileBrowse()],
            [sg.Button('Go'), sg.Button('Exit')]
        ]

        event, values = sg.Window(
            'Insira o caminho para a sua DB', create_DB_screen).read(close=True)

        if event == 'Go':
            sg.user_settings_set_entry(
                '-filenames-', list(set(sg.user_settings_get_entry('-filenames-', []) + [values['-FILENAME-'], ])))
            sg.user_settings_set_entry('-last filename-', values['-FILENAME-'])
            saida_c = subprocess.run(
                [self.DB_PATH, '-c', values['-FILENAME-'], '-1'], capture_output=True)
            print(saida_c.stdout.decode('utf-8'))
            if saida_c.returncode == 0:
                return True
            else:
                return False

    def load_fields(self):
        """Cria os parametros para abrir a janela"""

        gen = subprocess.run([self.DB_PATH, '-f', 'gen'],
                             capture_output=True).stdout.decode('utf-8').split(';')
        self.generos = sorted(gen)[3:]

        lan = subprocess.run([self.DB_PATH, '-f', 'lan'],
                             capture_output=True).stdout.decode('utf-8').split(';')
        self.idiomas = sorted(lan)[3:]

        tag = subprocess.run([self.DB_PATH, '-f', 'tag'],
                             capture_output=True).stdout.decode('utf-8').split(';')
        self.tags = sorted(tag)[3:]

        dev = subprocess.run([self.DB_PATH, '-f', 'dev'],
                             capture_output=True).stdout.decode('utf-8').split(';')
        self.devs = sorted(dev)[3:]

        pub = subprocess.run([self.DB_PATH, '-f', 'pub'],
                             capture_output=True).stdout.decode('utf-8').split(';')
        self.publishers = sorted(pub)[4:]

        dat = subprocess.run([self.DB_PATH, '-f', 'dat'],
                             capture_output=True).stdout.decode('utf-8').split(';')
        self.datas = sorted(dat)[2:]

        # Lista de decadas
        self.decadas = list(
            range(int(self.datas[0])-1, int(self.datas[len(self.datas)-3])+10, 10))
        maxprice = subprocess.run([self.DB_PATH, '-f', 'pri'],
                                  capture_output=True).stdout.decode('utf-8').split(';')
        self.max_price = float(0)
        for i in maxprice:
            if i != '' and i != '\r\n':
                if float(i)/100 > self.max_price:
                    self.max_price = float(i)/100
        
    def translate_reviews(self, values: list[bool]):
        """Traduz os valores de reviews para o formato da base de dados"""
        reviews = []
        if values[0]:
            reviews.append('Overwhelmingly Positive')
        if values[1]:
            reviews.append('Very Positive')
        if values[2]:
            reviews.append('Positive')
        if values[3]:
            reviews.append('Mostly Positive')
        if values[4]:
            reviews.append('Mixed')
        if values[5]:
            reviews.append('Mostly Negative')
        if values[6]:
            reviews.append('Negative')
        if values[7]:
            reviews.append('Very Negative')
        if values[8]:
            reviews.append('Overwhelmingly Negative')
        return reviews


    def searchDatabase(self, values):
        selected_date = values['-DATA-']
        selected_id = values['-ID-']
        selected_name = values['-NAME-']
        selected_price_min = -1#int(values['-PRICE_MIN-'] * 100) #TODO: Implementar Free como -1
        selected_price_max = -1#int(values['-PRICE_MAX-'] * 100) #TODO: Implementar Ilimitado como -1
        selected_genero = values['-GENERO-']
        selected_idioma = values['-IDIOMA-']
        selected_tags = values['-TAGS-']
        selected_dev = values['-DEV-']
        selected_publisher = values['-PUBLISHER-']
        selected_review = self.translate_reviews([values['-REV_EX_POS-'], values['-REV_MUITO_POS-'], values['-REV_POS-'], values['-REV_LIG_POS-'],
                           values['-REV_NEUTRAS-'], values['-REV_LIG_NEG-'], values['-REV_NEG-'], values['-REV_MUITO_NEG-'], values['-REV_EX_NEG-']])
        selected_decade = values['-DECADE-']
        selected_searchType = 0 # values['-SEARCH_TYPE-'] # 0 = AND, 1 = OR #TODO: Implementar OR e botão

        output = f'gen={selected_genero};lan={selected_idioma};tag={selected_tags};dat={selected_date};dev={selected_dev};pub={selected_publisher};min={selected_price_min};max={selected_price_max};dec={selected_decade};rev={selected_review};gid={selected_id};nam={selected_name};typ={selected_searchType}'
        print(output)
        print("running database.exe")
        saida_c = subprocess.run(
            ['./code/database.exe', '-s', output], capture_output=True)
        print(saida_c.stdout.decode('utf-8'))

    def cleanFields(self):
        self.window_menu['-ID-'].update('')
        self.window_menu['-NAME-'].update('')
        self.window_menu['-PRICE_MIN-'].update(0)
        self.window_menu['-PRICE_MAX-'].update(self.max_price)
        self.window_menu['-GENERO-'].update(values=self.generos)
        self.window_menu['-IDIOMA-'].update(values=self.idiomas)
        self.window_menu['-TAGS-'].update(values=self.tags)
        self.window_menu['-DATA-'].update(values=self.datas)
        self.window_menu['-DEV-'].update(values=self.devs)
        self.window_menu['-PUBLISHER-'].update(values=self.publishers)
        self.window_menu['-REV_EX_POS-'].update(False)
        self.window_menu['-REV_MUITO_POS-'].update(False)
        self.window_menu['-REV_POS-'].update(False)
        self.window_menu['-REV_LIG_POS-'].update(False)
        self.window_menu['-REV_NEUTRAS-'].update(False)
        self.window_menu['-REV_LIG_NEG-'].update(False)
        self.window_menu['-REV_NEG-'].update(False)
        self.window_menu['-REV_MUITO_NEG-'].update(False)
        self.window_menu['-REV_EX_NEG-'].update(False)
        self.window_menu['-DECADE-'].update(values=self.decadas)

    def main_window(self):
        while True:
            event, values = self.window_menu.read()
            if event == sg.WIN_CLOSED:
                break
            elif event == 'BUSCAR':
                self.searchDatabase(values)
            elif event == 'Limpar Filtros':
                self.cleanFields()

    def create_menu(self):
        """Layout da janela principal"""
        self.menu = [
            [
                sg.Text('STEAM BUG'),
            ],
            [
                sg.Text('GENERO', justification="center",
                        size=self.block_column_size-3),
                sg.Text('IDIOMA', justification="center",
                        size=self.block_column_size-3),
                sg.Text('TAGS', justification="center",
                        size=self.block_column_size-3),
                sg.Text('DATA', justification="center",
                        size=self.block_column_size-3),
                sg.Text('DEV', justification="center",
                        size=self.block_column_size-3),
                sg.Text('PUBLISHER', justification="center",
                        size=self.block_column_size-3)
            ],
            [
                sg.Listbox(values=self.generos, select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-GENERO-'),
                sg.Listbox(values=self.idiomas, select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-IDIOMA-'),
                sg.Listbox(values=self.tags, select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-TAGS-'),
                sg.Listbox(values=self.datas, select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-DATA-'),
                sg.Listbox(values=self.devs, select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-DEV-'),
                sg.Listbox(values=self.publishers, select_mode=sg.SELECT_MODE_MULTIPLE, size=(
                    self.block_column_size - 3, self.block_line_size), key='-PUBLISHER-')
            ],
            [
                sg.Push(), sg.Text('Preço MIN:', justification="center"), sg.Push(), sg.Text(
                    'Preço MAX:', justification="center"), sg.Push()
            ],
            [
                sg.Slider(orientation="h", range=(0, self.max_price), resolution=0.01, key='-PRICE_MIN-'), sg.Slider(
                    orientation="h", range=(0, self.max_price), resolution=0.01, default_value=self.max_price, key='-PRICE_MAX-')
            ],
            [
                sg.Text('Década:'), sg.Combo(
                    values=self.decadas, key='-DECADE-')
            ],
            [
                [sg.Text('Review:')],
                [
                    sg.Checkbox('Extremamente Positivas',
                                size=self.block_column_size, key='-REV_EX_POS-',),
                    sg.Checkbox(
                        'Muito Positivas', size=self.block_column_size, key='-REV_MUITO_POS-'),
                    sg.Checkbox(
                        'Positivas', size=self.block_column_size, key='-REV_POS-')
                ],
                [
                    sg.Checkbox('Ligeiramente Positivas',
                                size=self.block_column_size, key='-REV_LIG_POS-'),
                    sg.Checkbox(
                        'Neutras', size=self.block_column_size, key='-REV_NEUTRAS-'),
                    sg.Checkbox('Ligeiramente Negativas',
                                size=self.block_column_size, key='-REV_LIG_NEG-')
                ],
                [
                    sg.Checkbox(
                        'Negativas', size=self.block_column_size, key='-REV_NEG-'),
                    sg.Checkbox(
                        'Muito Negativas', size=self.block_column_size, key='-REV_MUITO_NEG-'),
                    sg.Checkbox('Extremamente Negativas',
                                size=self.block_column_size, key='-REV_EX_NEG-')
                ]
            ],
            [
                sg.Text('AppID:'), sg.InputText(key='-ID-', size=10),
                sg.Text('Nome:'), sg.InputText(key='-NAME-')
            ],
            [
                sg.Button('BUSCAR'),
                sg.Button('Limpar Filtros')
            ]
        ]


windows("./code/database.exe")
