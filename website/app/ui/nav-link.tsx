export default function NavLink({link, text, blank = false} : {link: string, text: string, blank?: boolean})
{
    if (blank === true)
    {
        return (<a target="_blank" className="ml-0 text-xl hover:underline" href={link}>{text}</a>)
    }
    else
    {
        return (<a className="ml-0 text-xl hover:underline" href={link}>{text}</a>)
    }
}