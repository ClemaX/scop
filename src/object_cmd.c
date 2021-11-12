#include <string.h>
#include <ctype.h>

#include <object.h>

#include <logger.h>

static void				object_cmd_perror(const object_cmd *cmd,
	const char *command, const char *arguments)
{
	if (cmd != NULL)
		error("object_load: %s %s: Invalid arguments\nUsage: %s %s\n",
			cmd->name, arguments, cmd->name, cmd->spec);
	else
		error("object_load: %s: Invalid command\n", command);
}

static const object_cmd	*object_cmd_parse(const char *command_name)
{
	static const object_cmd	commands[] = {
		{ "v",	"x, y, z [,w] (w defaults to 1.0)",	&object_v },
		{ "vn",	"x, y, z", &object_vn },
		{ "vt",	"u [,v ,w] (v and w default to 0)", &object_vt },
		{ "f",	"v1/vt1[/vn1] v2/vt2[/vn2] v3/vt3[/vn3] [...]", &object_f },
	};
	unsigned				i;

	debug("object_cmd_parse: '%s'\n", command_name);

	for (i = 0; i < sizeof(commands) / sizeof(*commands)
		&& strcmp(commands[i].name, command_name); i++)
		;

	if (i == sizeof(commands) / sizeof(*commands))
		return NULL;

	return commands + i;
}

int						object_exec(object *object, char *command)
{
	const object_cmd	*cmd;
	char				*arguments;
	int					ret;

	// Skip leading whitespace
	while (isspace(*command))
		command++;

	// Find the end of the command
	for (arguments = command; *arguments != '\0'
		&& !isspace(*arguments); arguments++)
		;

	// Check if there are arguments
	ret = *arguments == '\0';
	if (ret == 0)
		*arguments++ = '\0';

	// Find the start of the arguments
	while (isspace(*arguments))
		arguments++;

	// Check if there is any content to parse
	if (*command == '\0' || *command == '#')
		return 0;

	// Try to parse the command
	cmd = object_cmd_parse(command);
	if (ret == 0)
	{
		ret = (cmd == NULL);
		if (ret == 0)
		{
			debug("obj: executing '%s' '%s'...\n", cmd->name, arguments);
			ret = cmd->exec(object, arguments);
		}
	}

	if (ret == 1)
		object_cmd_perror(cmd, command, arguments);

	return ret;
}
