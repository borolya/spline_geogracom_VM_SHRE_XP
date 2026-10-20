static int message_number = 0;
static const char* message_file = "pech_messages.tsv";

static void reset_message_trace()
{
    FILE* file = fopen(message_file, "wt");

    if (file != NULL) {
        fprintf(file, "number\tfile\tline\tmessage\n");
        fclose(file);
    }

    message_number = 0;
}

static void traced_show_message(
    const AnsiString& message,
    const char* source_file,
    int source_line
)
{
    FILE* file = fopen(message_file, "at");

    if (file != NULL) {
        fprintf(
            file,
            "%d\t%s\t%d\t%s\n",
            ++message_number,
            source_file,
            source_line,
            message.c_str()
        );
        fclose(file);
    }

    // Окно по-прежнему показывается пользователю.
    ::ShowMessage(message);
}

#define ShowMessage(message) \
    traced_show_message((message), __FILE__, __LINE__)