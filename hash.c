/*
 * hash.c
 *
 *  Created on: Mar 26, 2024
 *      Author: Aidan Mondress
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct item
{
    int key;
    int h_val;
    struct item* next;
    struct item* last;
} item;

typedef struct hashTable
{
    int size;
    int count;
    item** items;
} hashTable;

hashTable* makeTable(int n)
{
    hashTable* table = (hashTable*)malloc(sizeof(table));
    table->size = n;
    table->count = 0;
    table->items = (item**)calloc(table->size, sizeof(node*));
    return table;
}

item* makeItem(int key, int h_val)
{
    item* _item = (item*)malloc(sizeof(item));
    _item->key = key;
    _item->h_val = h_val;
    _item->next = NULL;
    _item->last = NULL;
    return _item;
}

int hashFunction(int key, int n, int op, int a)
{
    int hashIndex;
    if(op == 0)
        hashIndex = (key * a) % n;
    else
        hashIndex = key % n;
    return hashIndex;
}

void insert(int key, int h_val, hashTable* table)
{
    item* _item = makeItem(key, h_val);
    int i = hashFunction(key, table->size);
    if(!table->items[i])
        table->items[i] = _item;
    else
    {
        item* temp = table->items[i];
        while(temp->next)
            temp = temp->next;
        temp->next = _item;
        _item->last = temp;
    }
    table->count++;
    if(table->count == table->size)
        rehash(table, table->size * 2);
}

void rehash(hashTable* table, int n)
{
    item** items = (item**)calloc(n, sizeof(item*));
    int n_prev = table->size;
    item** items_prev = table->items;
    table->size = n;
    table->items = items;
    table->count = 0;
    for(int i = 0; i < n_prev; i++)
    {
        item* _item = items[i];
        while(_item)
        {
            insert(table, _item->key, _item->h_val);
            item* item_prev = _item;
            _item = _item->next;
        }
    }
}

void delete(hashTable* table, int key)
{
    int index = hashFunction(key, table->size);
    item* _item = table->items[index];
    if(_item)
    {
        if(_item->key == key)
        {
            if(_item->next)
            {
            	_item->next->last = NULL;
                table->items[index] = _item->next;
            }
            else
                table->items[index] = NULL;
            table->count--;
            if(table->count == table->size / 4)
                rehash(table, table->size / 2);
        }
        else
        {
            while(_item->next)
            {
                if(_item->next->key == key)
                {
                    item* temp = _item->next;
                    _item->next = temp->next;
                    if (temp->next)
                        temp->next->last = _item;
                    table->count--;
                    if (table->count == table->size / 4)
                        rehash(table, table->size / 2);
                    break;
                }
                _item = _item->next;
            }
        }
    }
}

int main()
{
    hashTable* table = createHashTable(10);

    insert(table, 1, 20);
    insert(table, 2, 44);
    insert(table, 3, 9);
    insert(table, 4, 13);
    insert(table, 5, 39);

    delete(table, 4);

    return 0;
}
