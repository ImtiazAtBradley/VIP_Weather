import { Inter , Lusitana, Red_Hat_Display } from 'next/font/google';
 
export const inter = Inter({ subsets: ['latin'] });

export const RedHatDisplay = Red_Hat_Display({
  weight : ['500', '700'],
  subsets : ['latin']
})

export const lusitana = Lusitana({
    weight: ['400', '700'],
    subsets: ['latin'],
  });