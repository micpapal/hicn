/*
 * Copyright (c) 2017-2019 Cisco and/or its affiliates.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <src/config.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include <parc/algol/parc_Memory.h>
#include <parc/assert/parc_Assert.h>
#include <src/config/controlMapMeEnable.h>

#include <src/utils/commands.h>
#include <src/utils/utils.h>

static CommandReturn _controlMapMeEnable_Execute(CommandParser *parser,
                                                 CommandOps *ops,
                                                 PARCList *args);
static CommandReturn _controlMapMeEnable_HelpExecute(CommandParser *parser,
                                                     CommandOps *ops,
                                                     PARCList *args);

static const char *_commandMapMeEnable = "mapme enable";
static const char *_commandMapMeEnableHelp = "help mapme enable";

// ====================================================

CommandOps *controlMapMeEnable_Create(ControlState *state) {
  return commandOps_Create(state, _commandMapMeEnable, NULL,
                           _controlMapMeEnable_Execute, commandOps_Destroy);
}

CommandOps *controlMapMeEnable_HelpCreate(ControlState *state) {
  return commandOps_Create(state, _commandMapMeEnableHelp, NULL,
                           _controlMapMeEnable_HelpExecute, commandOps_Destroy);
}

// ====================================================

static CommandReturn _controlMapMeEnable_HelpExecute(CommandParser *parser,
                                                     CommandOps *ops,
                                                     PARCList *args) {
  printf("mapme enable [on|off]\n");
  printf("\n");

  return CommandReturn_Success;
}

static CommandReturn _controlMapMeEnable_Execute(CommandParser *parser,
                                                 CommandOps *ops,
                                                 PARCList *args) {
  if (parcList_Size(args) != 3) {
    _controlMapMeEnable_HelpExecute(parser, ops, args);
    return CommandReturn_Failure;
  }

  bool active;
  if (strcmp(parcList_GetAtIndex(args, 2), "on") == 0) {
    active = true;
  } else if (strcmp(parcList_GetAtIndex(args, 2), "off") == 0) {
    active = false;
  } else {
    _controlMapMeEnable_HelpExecute(parser, ops, args);
    return CommandReturn_Failure;
  }

  mapme_activator_command *mapmeEnableCommand =
      parcMemory_AllocateAndClear(sizeof(mapme_activator_command));
  if (active) {
    mapmeEnableCommand->activate = ACTIVATE_ON;
  } else {
    mapmeEnableCommand->activate = ACTIVATE_OFF;
  }

  ControlState *state = ops->closure;
  // send message and receive response
  struct iovec *response = utils_SendRequest(
      state, MAPME_ENABLE, mapmeEnableCommand, sizeof(mapme_activator_command));

  if (!response) {  // get NULL pointer
    return CommandReturn_Failure;
  }

  parcMemory_Deallocate(&response);  // free iovec pointer
  return CommandReturn_Success;
}
