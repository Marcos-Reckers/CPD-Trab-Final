import database
from GUI import windows

class Program:
    def __init__(self) -> None:
        self.windowManager = windows()

    def run(self):
        securityChecks = True
        while securityChecks:
            if not database.Validate():
                csv_path = self.windowManager.create_DB()
                time = database.Convert(csv_path, -1)
                if "Failed" in time:
                    self.windowManager.fail_popup("Vapor: Arquivo CSV Invalido", "O arquivo CSV selecionado não é valido.\n\nTente novamente.")
                else:
                    time = time.removeprefix("Converted database in ")
                    time = time.removesuffix(" ms")
                    self.windowManager.fail_popup("Vapor: Base de Dados Criada", f"A base de dados foi criada com sucesso em {time} milisegundos\n\nClique para prosseguir")
                continue

            securityChecks = False

        self.windowManager.search()


if __name__ == "__main__":
    main = Program()
    main.run()
