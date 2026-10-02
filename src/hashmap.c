/*
 * Copyright (C) 2026 TheCRatTime
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <hashmap.h>

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define INIT_MAP_SIZE 50
#define FNV1A_START   0x811C9DC5
#define FNV1A_PRIME   16777619

MapNode* free_map = NULL;

typedef unsigned long u_long;

static u_long FNV1AHash(const char* str) {
  u_long hash = FNV1A_START;
  while(*str) {
    hash ^= *str;
    hash *= FNV1A_PRIME;
    str++;
  }
  /* Set 32-bit integer always */
  return hash & 0xFFFFFFFF;
}

static int MapIdx(const char* str) {
  return FNV1AHash(str) % INIT_MAP_SIZE;
}

int InitFreeMap(void) {
  int i;
  free_map = (MapNode*)malloc(sizeof(MapNode) * INIT_MAP_SIZE);
  if (!free_map) {
    return 1;
  }

  for(i = 0; i < INIT_MAP_SIZE; i++) {
    free_map[i].key  = NULL;
    free_map[i].val  = NULL;
    free_map[i].next = NULL;
  }

  return 0;
}

void SetTrace(char *key, size_t len) {
  MapNode* node = NULL;
  if (key == NULL || len <= 0) {
    return;
  }

  node = &free_map[MapIdx(key)];
  while(node->next != NULL) {
    node = node->next;
  }
  
  if (node->key == NULL) {
  set_key_val:
    node->val = malloc(sizeof(size_t));
    if (node->val == NULL) {
      return;
    }
    *(size_t*)(node->val) = len;
    
    node->key = key;
  } else {
    node->next = (MapNode*)malloc(sizeof(MapNode));
    if (node->next == NULL) {
      return;
    }

    node->next->key  = NULL;
    node->next->val  = NULL;
    node->next->next = NULL;
    node = node->next;
    goto set_key_val;
  }
}

int FreeMapFind(const char *key) {
  MapNode* node = NULL;
  if (key == NULL) {
    return 0;
  }

  node = &free_map[MapIdx(key)];
  while(node != NULL) {
    if (node->key == NULL && node->next == NULL) {
      return 0;
    }

    if (node->key == NULL) {
      node = node->next;
      continue;
    }

    /* strcmp returns 0 at equal strings, other -
       not equals. 1 ISN'T mean strings equals */
    if (strcmp(node->key, key)) {
      if (node->next) {
        node = node->next;
        continue;
      }
      
      return 0;
    }

    return 1;
  }

  return 0;
}

MapNode* FreeMapGet(const char *key) {
  MapNode* node = NULL;
  if (key == NULL) {
    return NULL;
  }

  node = &free_map[MapIdx(key)];
  while(node != NULL) {
    if (node->key == NULL && node->next == NULL) {
      return NULL;
    }

    if (node->key == NULL) {
      node = node->next;
      continue;
    }
    
    if (strcmp(node->key, key)) {
      if (node->next) {
        node = node->next;
        continue;
      }
      
      return NULL;
    }
    
    return node;
  }

  return NULL;
}

void FreeFreeMap(void) {
  int i;
  for(i = 0; i < INIT_MAP_SIZE; i++) {
    MapNode* node = &free_map[i];
    while(node) {
      MapNode* saved = node;
      node->key = NULL;
      node->val = NULL;
      if (node->next) {
        node = node->next;
      }
      free(saved);
    }
  }
}
