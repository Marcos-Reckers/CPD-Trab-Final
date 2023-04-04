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
                print(time)
                continue

            securityChecks = False

        fields: dict = {}
        fields["languages"] = self.windowManager.fields('language')
        fields["genres"] = self.windowManager.fields('genre')
        fields["tags"] = self.windowManager.fields('tag')
        fields["developers"] = self.windowManager.fields('developer')
        fields["publishers"] = self.windowManager.fields('publisher')
        fields["dates"] = self.windowManager.fields('date')
        fields["price"] = self.windowManager.fields('price')[0]
        fields["decades"] = self.windowManager.fields('decade')

        self.windowManager.search(fields)


if __name__ == "__main__":
    main = Program()
    main.run()
