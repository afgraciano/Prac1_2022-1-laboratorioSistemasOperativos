#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/cdev.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>
#include "bridge.h"
#include <linux/random.h>

int bridge_major =   BRIDGE_MAJOR;
int bridge_minor =   0;
int bridge_nr_devs = BRIDGE_NR_DEVS;	/* number of bare bridge devices */

module_param(bridge_major, int, S_IRUGO);
module_param(bridge_minor, int, S_IRUGO);
module_param(bridge_nr_devs, int, S_IRUGO);

MODULE_AUTHOR("Jheisson Argiro Lopez Restrepo");
MODULE_LICENSE("Dual BSD/GPL");

LIST_HEAD(stack);
LIST_HEAD(lista);//simple_list
LIST_HEAD(lista2);//simple_lista_2

LIST_HEAD(cola_alta);//hight_Q
LIST_HEAD(cola_media);//middle_Q
LIST_HEAD(cola_baja);//low_Q

static void add_element_to_list(char *node_element_msg){
	struct string_node *tmp_element;
	tmp_element = kmalloc(sizeof(struct string_node), GFP_KERNEL);
	strcpy(tmp_element->message, node_element_msg);
	INIT_LIST_HEAD(&tmp_element->list);
	list_add_tail(&(tmp_element->list), &lista);
}

static void add_element_to_list_2(char *node_element_msg){
	struct string_node *tmp_element;
	tmp_element = kmalloc(sizeof(struct string_node), GFP_KERNEL);
	strcpy(tmp_element->message, node_element_msg);
	INIT_LIST_HEAD(&tmp_element->list);
	list_add_tail(&(tmp_element->list), &lista2);
}


static void add_element_to_cola_alta(char *node_element_msg)//add_to_hight_prior_q
{
	struct string_node *tmp_element;
	tmp_element = kmalloc(sizeof(struct string_node), GFP_KERNEL);
	strcpy(tmp_element->message, node_element_msg);
	INIT_LIST_HEAD(&tmp_element->list);
	list_add_tail(&(tmp_element->list), &cola_alta);
}

static void add_element_to_cola_media(char *node_element_msg)//add_to_middle_prior_q
{
	struct string_node *tmp_element;
	tmp_element = kmalloc(sizeof(struct string_node), GFP_KERNEL);
	strcpy(tmp_element->message, node_element_msg);
	INIT_LIST_HEAD(&tmp_element->list);
	list_add_tail(&(tmp_element->list), &cola_media);
}

static void add_element_to_cola_baja(char *node_element_msg)//add_to_low_prior_q
{
	struct string_node *tmp_element;
	tmp_element = kmalloc(sizeof(struct string_node), GFP_KERNEL);
	strcpy(tmp_element->message, node_element_msg);
	INIT_LIST_HEAD(&tmp_element->list);
	list_add_tail(&(tmp_element->list), &cola_baja);
}

static void add_element_to_stack(char *node_element_msg)
{
	struct string_node *tmp_element;
	tmp_element = kmalloc(sizeof(struct string_node), GFP_KERNEL);
	strcpy(tmp_element->message, node_element_msg);
	INIT_LIST_HEAD(&tmp_element->list);		
	list_add(&(tmp_element->list), &stack);
}

static void invertir_lista(void)//invert_list
{
	struct string_node *tmp_element;
	struct list_head *watch, *prev;
	list_for_each_prev_safe(watch, prev, &lista)
	{
	tmp_element = list_entry(watch, struct string_node, list);
	printk(KERN_INFO "%s", tmp_element->message);
	list_del(&(tmp_element->list));
	list_add_tail(&(tmp_element->list), &lista);
	}
}

void rotar_lista(void)//rotate_list

	{
        struct string_node *tmp_element;
		char message[100];
		tmp_element = list_last_entry(&lista, struct string_node, list);
        list_del(&(tmp_element->list));
		list_add(&(tmp_element->list), &lista);
   }

static int buscar_duplicado(char *node_element_msg)//search_duplicated

	{
		if(list_empty(&lista) == 0){
		struct string_node *tmp_element;
		struct list_head *watch, *next;
		int c = 0;//cont = 0
		list_for_each_safe(watch, next, &lista)
		{
			tmp_element = list_entry(watch, struct string_node, list);
			if(strcmp(tmp_element->message, node_element_msg) == 0)
			{
				printk(KERN_INFO "\nENCONTRO DUPLICADO\n");
				c = c + 1; //cont = cont + 1

			}
		}
		if(c > 1)//cont > 1
		{
			return 1;
		}

		}	

        return 0;
		
   }


static void concatenar_listas(void) //concat_lists
	{
		struct string_node *tmp_element;
		struct list_head *watch, *next;
		list_for_each_safe(watch, next, &cola_alta)
		{
			tmp_element = list_entry(watch, struct string_node, list);
			list_del(&(tmp_element->list));
			list_add_tail(&(tmp_element->list), &lista);
		}
	}


void borrar_lista(void)//delete_list

	{
		struct string_node *tmp_element;
		struct list_head *watch, *next;
		list_for_each_safe(watch, next, &lista)
		{
				tmp_element = list_entry(watch, struct string_node, list);
				list_del(&(tmp_element->list));
			kfree(tmp_element);
		}
			kfree(&lista);
	}



static void borrar_lista_entradas_repetidas(void) //delete_repeated_entries_list
	{
		struct string_node *tmp_element;
		struct list_head *watch, *next;
		list_for_each_safe(watch, next, &lista)
		{
			tmp_element = list_entry(watch, struct string_node, list);
			
			if(buscar_duplicado(tmp_element->message) == 1)
			{
				list_del(&(tmp_element->list));
				kfree(tmp_element);
			}
		}
	}

static void concatenar_dos_listas(void)//concat_two_list
	{
		list_splice(&lista2, &lista);
	}

void misalida_pila(void)//mystack_exit
{
    struct string_node *tmp_element;
	struct list_head *watch, *next;
	list_for_each_safe(watch, next, &stack)
	{
        	tmp_element = list_entry(watch, struct string_node, list);
        	list_del(&(tmp_element->list));
		kfree(tmp_element);
     	}
		printk(KERN_INFO "\nPILA VACIADA\n");
     	//kfree(&stack);
}

void mylist_exit(void){
    struct string_node *tmp_element;
	struct list_head *watch, *next;
	list_for_each_safe(watch, next, &lista)
	{
        	tmp_element = list_entry(watch, struct string_node, list);
        	list_del(&(tmp_element->list));
		kfree(tmp_element);
     	}
     	printk(KERN_INFO "\nLISTA VACIADA\n");
     	//kfree(&stack);
}





/*void mylist_exit(void){
    struct string_node *tmp_element;
	struct list_head *watch, *next;
	list_for_each_safe(watch, next, &stack){
        	tmp_element = list_entry(watch, struct string_node, list);
        	list_del(&(tmp_element->list));
		kfree(tmp_element);
     	}
     	//kfree(&stack);
}
*/
struct bridge_dev *bridge_devices;	/* allocated in bridge_init_module */

static long bridge_ioctl(struct file *f, unsigned int cmd, unsigned long arg){
    int return_value = 0;
    int data;
    char message[100];
    struct string_node *tmp_element;
    struct complex_struct tmp;
    switch(cmd){
	case BRIDGE_CREATE_Q:
            printk(KERN_INFO "message %s\n", "bla");
	    //Return a posituve value indicating the state of the queue
	    return_value = 1;
	    break;
	case BRIDGE_W_HIGH_PRIOR_Q:
        raw_copy_from_user(message, (char *)arg, 100);
	    add_element_to_cola_alta(message);
		printk(KERN_INFO "message %s\n", "Cola de Alta Prioridad");
	    break;
	case BRIDGE_W_MIDDLE_PRIOR_Q:
        raw_copy_from_user(message, (char *)arg, 100);
	    add_element_to_cola_media(message);
		printk(KERN_INFO "message %s\n", "Cola de Media Prioridad");
	    break;
	case BRIDGE_W_LOW_PRIOR_Q:
        raw_copy_from_user(message, (char *)arg, 100);
	    add_element_to_cola_baja(message);
		printk(KERN_INFO "message %s\n", "Cola de Baja Prioridad");
	    break;
	case BRIDGE_R_HIGH_PRIOR_Q:
        tmp_element = list_first_entry(&cola_alta, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_R_MIDDLE_PRIOR_Q:
        tmp_element = list_first_entry(&cola_media, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_R_LOW_PRIOR_Q:
        tmp_element = list_first_entry(&cola_baja, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_STATE_Q:
            
            if(list_empty(&cola_alta) != 0){
            	if(list_empty(&cola_media) != 0){
            		if(list_empty(&cola_baja) !=0){
            		return_value = 0;//retorna cero si ninguna cola tiene elementos
            		
            		}else{
            		
            		return_value=3;//retorna 3 si la cola de baja prioridad tiene elementos pero no hay en alta o media
            		}
            	}else{ 
            	
            	return_value=2;//retorna 2 si la cola de media prioridad tiene elementos pero no hay en alta
            	}
		
	    }else{
		return_value = 1;//retorna 1 si la cola de alta prioridad tiene elementos
	    }
            
            
            printk(KERN_INFO "Estado de lista simple enviado con éxito");
	    break;
	case BRIDGE_DESTROY_Q:
            
            
            printk(KERN_INFO "message %s\n", "bla7");
	    break;

	case BRIDGE_CREATE_S:
	    printk(KERN_INFO "Stack succesfully created\n");
	    break;
	case BRIDGE_W_S:
        raw_copy_from_user(message, (char *)arg, 100);
	    add_element_to_stack(message);
        printk(KERN_INFO "Element succesfully added to the stack\n");
	    break;
	case BRIDGE_R_S:
	    tmp_element = list_first_entry(&stack, struct string_node, list);
        list_del(&(tmp_element->list));
	    raw_copy_to_user((char *)arg, tmp_element->message, 100);
	    kfree(tmp_element);
	    break;
	case BRIDGE_STATE_S:
        if(list_empty(&stack) != 0)
		{
			return_value = 0;
	    }else
		{
			return_value = 1;
	    }
	    printk(KERN_INFO "Stack state succesfully sended!!!\n");
	    break;
	case BRIDGE_DESTROY_S:
	    misalida_pila();
            //printk(KERN_INFO "message %s\n", "bla12");
	    break;
	case BRIDGE_CREATE_L:
             printk(KERN_INFO "message %s\n", "bla13");
	     break;
	case BRIDGE_W_L:
        raw_copy_from_user(message, (char *)arg, 100);
 	    add_element_to_list(message);
		 printk(KERN_INFO "Elemento agregado con éxito a la lista simple");
	    break;
	case BRIDGE_W_L2:
        raw_copy_from_user(message, (char *)arg, 100);
 	    add_element_to_list_2(message);
		 printk(KERN_INFO "Elemento agregado con éxito a la lista simple");
	    break;
	
	case BRIDGE_R_L:
         tmp_element = list_first_entry(&lista, struct string_node, list);
         list_del(&(tmp_element->list));
	     raw_copy_to_user((char *)arg, tmp_element->message, 100);
	     kfree(tmp_element);
	     break;
	case BRIDGE_INVERT_L:
            invertir_lista();
			// printk(KERN_INFO "message %s\n", "bla16");
	     break;
	case BRIDGE_ROTATE_L:
			rotar_lista();
			//get_user(data, (int *)arg);
        printk(KERN_INFO "message %d\n", data);
	    break;
	case BRIDGE_CLEAN_L:
			borrar_lista_entradas_repetidas();
             printk(KERN_INFO "Lista limpiada \n");
	     break;
	case BRIDGE_GREATER_VAL_L:
	     //strcpy((char *)arg, "MensajePrueba");
             printk(KERN_INFO "message %s\n", "bla19");
	     break;
	case BRIDGE_END_L:
             printk(KERN_INFO "message %s\n", "bla21");
	     break;
	case BRIDGE_CONCAT_L:
		concatenar_dos_listas();
             //printk(KERN_INFO "message %s\n", "bla22");
	     break;
	case BRIDGE_STATE_L:
	    if(list_empty(&lista) != 0){
		return_value = 0;
  	}else{
		return_value = 1;
	    }
		printk(KERN_INFO "\n¡¡¡Estado de lista simple enviado con éxito!!!\n");
	     break;
	case BRIDGE_DESTROY_L:
	borrar_lista();
             //printk(KERN_INFO "message %s\n", "bla24");
             break;
	case BRIDGE_W_CS:
	    raw_copy_from_user(&tmp, (struct complex_struct *)arg, sizeof(struct complex_struct));
	    printk(KERN_INFO "Message1 %s\n", tmp.messages[0]);
            printk(KERN_INFO "Message2 %s\n", tmp.messages[1]);
            printk(KERN_INFO "Message3 %s\n", tmp.messages[2]);

    }
    return return_value;
}

struct file_operations bridge_fops = {
	.owner =    THIS_MODULE,
	.unlocked_ioctl = bridge_ioctl
};

/*
 * Finally, the module stuff
 */

/*
 * The cleanup function is used to handle initialization failures as well.
 * Thefore, it must be careful to work correctly even if some of the items
 * have not been initialized
 */
void bridge_cleanup_module(void)
{
	int i;
	dev_t devno = MKDEV(bridge_major, bridge_minor);
	mylist_exit();
	/* Get rid of our char dev entries */
	if (bridge_devices) {
		for (i = 0; i < bridge_nr_devs; i++) {
			cdev_del(&bridge_devices[i].cdev);
		}
		//Be aware of clean memory completely
		kfree(bridge_devices);
	}

	/* cleanup_module is never called if registering failed */
	unregister_chrdev_region(devno, bridge_nr_devs);
}

/*
 * Set up the char_dev structure for this device.
 */
static void bridge_setup_cdev(struct bridge_dev *dev, int index)
{
	int err, devno;

	devno = MKDEV(bridge_major, bridge_minor + index);
	cdev_init(&dev->cdev, &bridge_fops);
	dev->cdev.owner = THIS_MODULE;
	err = cdev_add(&dev->cdev, devno, 1);
	/* Fail gracefully if need be */
	if (err)
		printk(KERN_NOTICE "Error %d adding bridge%d", err, index);
}


int bridge_init_module(void)
{
	int result, i;
	dev_t dev = 0;

/*
 * Get a range of minor numbers to work with, asking for a dynamic
 * major unless directed otherwise at load time.
 */
	if (bridge_major) {
		dev = MKDEV(bridge_major, bridge_minor);
		result = register_chrdev_region(dev, bridge_nr_devs, "bridge");
	} else {
		result = alloc_chrdev_region(&dev, bridge_minor, bridge_nr_devs, "bridge");
		bridge_major = MAJOR(dev);
        	bridge_devices = kmalloc(bridge_nr_devs * sizeof(struct bridge_dev), GFP_KERNEL);
	}

	if (result < 0) {
		printk(KERN_WARNING "bridge: can't get major %d\n", bridge_major);
		return result;
	}

		/*
	 * allocate the devices -- we can't have them static, as the number
	 * can be specified at load time
	 */
	bridge_devices = kmalloc(bridge_nr_devs * sizeof(struct bridge_dev), GFP_KERNEL);

	if (!bridge_devices) {
		result = -ENOMEM;
		goto fail;  /* Make this more graceful */
	}

	memset(bridge_devices, 0, bridge_nr_devs * sizeof(struct bridge_dev));

		/* Initialize each device. */
	for (i = 0; i < bridge_nr_devs; i++) {
		bridge_setup_cdev(&bridge_devices[i], i);
	}

	return 0; /* succeed */

  fail:
	bridge_cleanup_module();
	return result;
}

module_init(bridge_init_module);
module_exit(bridge_cleanup_module);
